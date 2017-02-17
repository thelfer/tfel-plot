/*!
 * \file   DataCurve.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 jan 2008
 */

#include<set>
#include<sstream>
#include<cassert>
#include<iterator>
#include<algorithm>
#include<stdexcept>

#include<QtCore/QtDebug>
#include<QtCore/QTimer>


#include"TFEL/Math/Evaluator.hxx"
#include"TFEL/Plot/TextDataReader.hxx"
#include"TFEL/Plot/DataCurve.hxx"

namespace tfel{

  namespace plot{

    DataCurve::DataCurve(const QString& f,
			 const unsigned short c1,
			 const unsigned short c2)
      : file(f),
	ucx(c1),
	ucy(c2)
    {
      this->watcher = new QFileSystemWatcher(this);
      this->watcher->addPath(this->file);
      this->loadDataFromFile(true);
      emit updated(this);
    } // end of DataCurve::DataCurve

    DataCurve::DataCurve(const QString& f,
			 const QString& c1,
			 const unsigned short c2,
			 DataCurve::ExternalFunctionManagerPtr functions)
      : fm(functions),
	file(f),
	cx(c1),
	ucx(0),
	ucy(c2)
    {
      this->watcher = new QFileSystemWatcher(this);
      this->watcher->addPath(this->file);
      this->loadDataFromFile(true);
      emit updated(this);
    } // end of DataCurve::DataCurve

    DataCurve::DataCurve(const QString& f,
			 const unsigned short c1,
			 const QString& c2,
			 DataCurve::ExternalFunctionManagerPtr functions)
      : fm(functions),
	file(f),
	cy(c2),
	ucx(c1),
	ucy(0)
    {
      this->watcher = new QFileSystemWatcher(this);
      this->watcher->addPath(this->file);
      this->loadDataFromFile(true);
      emit updated(this);
    } // end of DataCurve::DataCurve

    DataCurve::DataCurve(const QString& f,
			 const QString& c1,
			 const QString& c2,
			 DataCurve::ExternalFunctionManagerPtr functions)
      : fm(functions),
	file(f),
	cx(c1),
	cy(c2),
	ucx(0),
	ucy(0)
    {
      this->watcher = new QFileSystemWatcher(this);
      this->watcher->addPath(this->file);
      this->loadDataFromFile(true);
      emit updated(this);
    } // end of DataCurve::DataCurve

    DataCurve::DataCurve(const QVector<qreal>& x,
			 const QVector<qreal>& y)
      : file(""),
	ucx(0),
	ucy(0),
	xvalues(x),
	yvalues(y)
    {
      using namespace std;
      if(this->xvalues.size()!=this->yvalues.size()){
	string msg("DataCurve::DataCurve : ");
	msg += "x-data and y-data sizes do not match";
	throw(runtime_error(msg));
      }
    } // end of DataCurve::DataCurve

    bool DataCurve::hasRange() const
    {
      return !this->xvalues.empty();
    } // end of DataCurve::hasRange

    qreal DataCurve::minRange() const
    {
      return *(std::min_element(this->xvalues.begin(),this->xvalues.end()));
    } // end of DataCurve::minRange

    qreal DataCurve::maxRange() const
    {
      return *(std::max_element(this->xvalues.begin(),this->xvalues.end()));
    } // end of DataCurve::maxRange

    void DataCurve::getValues(QVector<Point>& points,
			      const qreal x0,
			      const qreal x1,
			      const unsigned short)
    {
      using namespace std;
      QVector<qreal>::size_type nbr;
      qreal rx0 = qMin(x0,x1);
      qreal rx1 = qMax(x0,x1);
      points.clear();
      if(this->xvalues.empty()){
	return;
      } else if(this->xvalues.size()==1u){
	const qreal x = this->xvalues.front();
	if((x>=rx0)&&(x<=rx1)){
	  Point point;
	  point.x = x;
	  point.y = this->yvalues.front();
	  points.push_back(point);
	}
      } else {
	bool pinc = false; // true if the previous segments was selected
	for(nbr=0u;nbr!=this->xvalues.size()-1;++nbr){
	  const qreal xl = this->xvalues[nbr];
	  const qreal xr = this->xvalues[nbr+1];
	  qreal rxl = qMin(xl,xr);
	  qreal rxr = qMax(xl,xr);
	  if(((rxl>=rx0)&&(rxl<=rx1))||
	     ((rxr>=rx0)&&(rxr<=rx1))||
	     ((rxl<=rx0)&&(rxr>=rx1))||
	     ((rx0<=rxl)&&(rx1>=rxr))){
	    Point point;
	    if(!pinc){
	      point.x = this->xvalues[nbr];
	      point.y = this->yvalues[nbr];
	      points.push_back(point);
	    }
	    point.x = this->xvalues[nbr+1];
	    point.y = this->yvalues[nbr+1];
	    points.push_back(point);
	    pinc=true;
	  } else {
	    pinc=false;
	  }
	}
      }
    } // end of DataCurve::getValues

    bool DataCurve::hasSpecifiedNumberOfSamples() const
    {
      return false;
    } // end of DataCurve::hasSpecifiedNumberOfSamples

    void DataCurve::setNumberOfSamples(const unsigned short,const bool b)
    {
      if(b){
	emit updated(this);
      }
    } // end of DataCurve::setNumberOfSamples

    unsigned short DataCurve::getNumberOfSamples() const
    {
      return 0u;
    } // end of DataCurve::getNumberOfSamples

    const QVector<qreal>&
    DataCurve::getAbscissa() const
    {
      return this->xvalues;
    } // end of DataCurve::getAbscissa

    const QVector<qreal>&
    DataCurve::getValues() const
    {
      return this->yvalues;
    } // end of DataCurve::getValues

    void DataCurve::updatedDataFile(const QString&)
    {
      this->loadDataFromFile(false);
      emit updated(this);
    } // end of DataCurve::updatedDataFile

    void DataCurve::getValues(QVector<double>& v,
			      const TextDataReader& d,
			      const QString& c)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      const auto& s = c.toStdString();
      v.clear();
      // assuming a function
      vector<pair<string,unsigned short> > vars;
      shared_ptr<Evaluator> e;
      if(this->fm.get()!=nullptr){
	e = std::make_shared<Evaluator>(s,this->fm);
      } else {
	e = std::make_shared<Evaluator>(s);
      }
      const auto& vnames = e->getVariablesNames();
      if(vnames.empty()){
	string msg("DataCurve::getValues : ");
	msg += "function '"+s+"' does not declare any variable";
	throw(runtime_error(msg));
      }
      vector<string>::const_iterator p;
      QVector<TextDataReader::Line>::const_iterator p2;
      vector<pair<string,unsigned short> >::const_iterator p3;
      for(p=vnames.begin();p!=vnames.end();++p){
	if(((*p)[0]!='$')){
	  parser::ExternalFunctionManager::const_iterator p4;
	  p4 = this->fm->find(*p);
	  if(p4==this->fm->end()){
	    string msg("DataCurve::getValues : ");
	    msg += "invalid variable '"+*p+"'";
	    throw(runtime_error(msg));
	  }
	  if(p4->second->getNumberOfVariables()!=0){
	    string msg("DataCurve::getValues : ");
	    msg += "invalid variable '"+*p+"'";
	    throw(runtime_error(msg));
	  }
	  e->setVariableValue(*p,p4->second->getValue());
	} else {
	  if(!DataCurve::isUnsignedInteger(p->substr(1))){
	    string msg("DataCurve::getValues : ");
	    msg += "invalid variable name '"+*p;
	    msg += "' in function '"+s+"'";
	    throw(runtime_error(msg));
	  }
	  const auto vc = DataCurve::convertToUnsignedShort(p->substr(1));
	  if(vc==0){
	    string msg("DataCurve::getValues : ");
	    msg += "invalid variable name "+*p;
	    msg += " in function '"+s+"'.";
	    throw(runtime_error(msg));
	  }
	  vars.push_back({*p,vc});
	}
      }
      for(p2=d.begin();p2!=d.end();++p2){
	for(p3=vars.begin();p3!=vars.end();++p3){
	  if(p2->values.size()<p3->second){
	    ostringstream msg;
	    msg << "TextDataReader::getColumn : line '" 
		<< p2->nbr << "' "
		<< "does not have '" << p3->second << "' columns.";
	    throw(runtime_error(msg.str()));
	  }
	  e->setVariableValue(p3->first,
			      p2->values[p3->second-1]);
	}
	v.push_back(e->getValue());
      }
    } // end of DataCurve::getValues

    void DataCurve::executeDelayedDataLoading()
    {
      using namespace std;
      QObject::connect(this->watcher,SIGNAL(fileChanged(const QString&)),
		       this,SLOT(updatedDataFile(const QString&)));
      try{
	TextDataReader data(this->file);
	if(this->cy.isEmpty()){
	  this->yvalues = data.getColumn(this->ucy);
	} else {
	  this->getValues(this->yvalues,data,this->cy);
	}
	if(this->cx.isEmpty()){
	  this->xvalues = data.getColumn(this->ucx);
	} else {
	  if(this->cx=="__DataCurve__::line_number"){
	    this->xvalues.resize(this->yvalues.size());
	    for(QVector<qreal>::size_type i=0;i!=this->xvalues.size();++i){
	      this->xvalues[i] = i;
	    }
	  } else {
	    this->getValues(this->xvalues,data,this->cx);
	  }
	}
      } catch(exception& e){
	qDebug() << "DataCurve::executeDelayedDataLoading : " << e.what();
	this->xvalues.clear();
	this->yvalues.clear();
      } catch(...){
	qDebug() << "DataCurve::executeDelayedDataLoading : " 
		 << "unknown exception";
	this->xvalues.clear();
	this->yvalues.clear();
      }
    }

    void DataCurve::loadDataFromFile(const bool first)
    {
      if(first){
	this->executeDelayedDataLoading();
      } else {
	// wait 100 micro-seconds for the data to be effectively ready
	QTimer::singleShot(100, this, SLOT(executeDelayedDataLoading()));
	QObject::disconnect(this->watcher,SIGNAL(fileChanged(const QString&)),
			    this,SLOT(updatedDataFile(const QString&)));
      }
    }

    unsigned short
    DataCurve::convertToUnsignedShort(const std::string& value)
    {
      std::istringstream converter(value);
      for(const auto& c : value){
	if(!isdigit(c)){
	  throw(std::runtime_error("DataCurve::convertToUnsignedShort: invalid entry"));
	}
      }
      unsigned short u;
      converter >> u;
      if(!converter&&(!converter.eof())){
	std::string msg("DataCurve::convertToUnsignedShort: ");
	msg += "not read value from token '"+value+"'.\n";
	throw(std::runtime_error(msg));
      }
      return u;
    } // end of DataCurve::convertToUnsignedShort

    bool DataCurve::isUnsignedInteger(const std::string& s)
    {
      using namespace std;
      string::const_iterator p;
      for(p=s.begin();p!=s.end();++p){
	if(!isdigit(*p)){
	  return false;
	}
      }
      return true;
    } // end of DataCurve::isUnsignedInteger

    double
    DataCurve::readDouble(const std::string& s,
			  const unsigned short l)
    {
      using namespace std;
      double res;
      istringstream is(s);
      is >> res;
      if(!is&&(!is.eof())){
	ostringstream msg;
	msg << "DataCurve::readDouble : ";
	msg << "could not read value from token '"+s+"'.\n";
	msg << "Error at line : " << l;
	throw(runtime_error(msg.str()));
      }
      return res;
    } // end of DataCurve::readDouble

    DataCurve::~DataCurve() = default;

  } // end of namespace plot

} // end of namespace tfel

