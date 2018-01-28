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
#include<QtCore/QFile>
#include<QtCore/QTimer>

#include"TFEL/Raise.hxx"
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
      tfel::raise_if(this->xvalues.size()!=this->yvalues.size(),
		     "DataCurve::DataCurve: "
		     "x-data and y-data sizes do not match");
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

    const QVector<qreal>& DataCurve::getAbscissa() const
    {
      return this->xvalues;
    } // end of DataCurve::getAbscissa

    const QVector<qreal>& DataCurve::getValues() const
    {
      return this->yvalues;
    } // end of DataCurve::getValues

    void DataCurve::updatedDataFile(const QString&)
    {
      this->watcher->removePath(this->file);
      if(!QFile(this->file).exists()){
	// method was called to say that the file has disapeared
	QTimer::singleShot(1000,this,[this]{
	    this->updatedDataFile(this->file);
	  });
      } else {
	this->watcher->addPath(this->file);
	this->loadDataFromFile(false);
      }
    } // end of DataCurve::updatedDataFile

    void DataCurve::getValues(QVector<double>& v,
			      const TextDataReader& d,
			      const QString& c)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      auto throw_if = [](const bool b, const std::string& msg){
	tfel::raise_if(b,"DataCurve::getValues: "+msg);
      }; // end of throw_if
      const auto& s = c.toStdString();
      v.clear();
      // assuming a function
      vector<pair<string,unsigned short>> vars;
      shared_ptr<Evaluator> e;
      if(this->fm.get()!=nullptr){
	e = std::make_shared<Evaluator>(s,this->fm);
      } else {
	e = std::make_shared<Evaluator>(s);
      }
      const auto& vnames = e->getVariablesNames();
      throw_if(vnames.empty(),
	       "function '"+s+"' does not declare any variable");
      for(const auto& vn : vnames){
	if(vn[0]!='$'){
	  const auto p4 = this->fm->find(vn);
	  throw_if(p4==this->fm->end(),"invalid variable '"+vn+"'");
	  throw_if(p4->second->getNumberOfVariables()!=0,
		   "invalid variable '"+vn+"'.");
	  e->setVariableValue(vn,p4->second->getValue());
	} else {
	  throw_if(!DataCurve::isUnsignedInteger(vn.substr(1)),
		   "invalid variable name '"+vn+"' in function '"+s+"'.");
	  const auto vc = DataCurve::convertToUnsignedShort(vn.substr(1));
	  throw_if(vc==0,"invalid variable name "
		   "'"+vn+"' in function '"+s+"').");
	  vars.push_back({vn,vc});
	}
      }
      for(auto p2=d.begin();p2!=d.end();++p2){
	for(auto p3=vars.begin();p3!=vars.end();++p3){
	  throw_if(p2->values.size()<p3->second,
		   "line '"+std::to_string(p2->nbr)+"' "
		   "does not have '"+std::to_string(p3->second)+"' columns.");
	  e->setVariableValue(p3->first,p2->values[p3->second-1]);
	}
	v.push_back(e->getValue());
      }
    } // end of DataCurve::getValues

    void DataCurve::executeDelayedDataLoading()
    {
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
      } catch(std::exception& e){
	qDebug() << "DataCurve::executeDelayedDataLoading : " << e.what();
	this->xvalues.clear();
	this->yvalues.clear();
      } catch(...){
	qDebug() << "DataCurve::executeDelayedDataLoading : " 
		 << "unknown exception";
	this->xvalues.clear();
	this->yvalues.clear();
      }
      emit(updated(this));
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
	tfel::raise_if(!isdigit(c),
		       "DataCurve::convertToUnsignedShort: "
		       "invalid entry");
      }
      unsigned short u;
      converter >> u;
      tfel::raise_if(!converter&&(!converter.eof()),
		     "DataCurve::convertToUnsignedShort: "
		     "not read value from token '"+value+"'.");
      return u;
    } // end of DataCurve::convertToUnsignedShort

    bool DataCurve::isUnsignedInteger(const std::string& s)
    {
      for(const auto c : s){
	if(!isdigit(c)){
	  return false;
	}
      }
      return true;
    } // end of DataCurve::isUnsignedInteger

    double DataCurve::readDouble(const std::string& s,
				 const unsigned short l)
    {
      std::istringstream is(s);
      double res;
      is >> res;
      tfel::raise_if(!is&&(!is.eof()),
		     "DataCurve::readDouble: "
		     "could not read value from token '"+s+"'.\n"
		     "Error at line: "+std::to_string(l));
      return res;
    } // end of DataCurve::readDouble

    DataCurve::~DataCurve() = default;

  } // end of namespace plot

} // end of namespace tfel

