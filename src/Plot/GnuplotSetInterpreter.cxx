/*! 
 * \file  GnuplotSetInterpreter.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#include<sstream>

#include<QtCore/QtDebug>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"TFEL/Plot/Grid.hxx"
#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/ThemeManager.hxx"
#include"TFEL/Plot/GnuplotSetInterpreter.hxx"

namespace tfel
{

  namespace plot
  {

    GnuplotInterpreter::SetInterpreter::SetInterpreter(GnuplotInterpreter& i,
						       Graph& graph)
      : GnuplotInterpreterBase(graph),
	interpreter(i)
    {
      this->registerCallBacks();
    } // end of GnuplotInterpreter::SetInterpreter::SetInterpreter

    void GnuplotInterpreter::SetInterpreter::registerCallBacks()
    {
      this->registerCallBack(this->setCallBacks,"border",
      			     &SetInterpreter::treatBorder);
      this->registerCallBack(this->setCallBacks,"noborder",
      			     &SetInterpreter::treatNoBorder);
      this->registerCallBack(this->setCallBacks,"xlabel",
      			     &SetInterpreter::treatXLabel);
      this->registerCallBack(this->setCallBacks,"xl",
      			     &SetInterpreter::treatXLabel);
      this->registerCallBack(this->setCallBacks,"x2label",
      			     &SetInterpreter::treatX2Label);
      this->registerCallBack(this->setCallBacks,"x2l",
      			     &SetInterpreter::treatX2Label);
      this->registerCallBack(this->setCallBacks,"ylabel",
      			     &SetInterpreter::treatYLabel);
      this->registerCallBack(this->setCallBacks,"yl",
      			     &SetInterpreter::treatYLabel);
      this->registerCallBack(this->setCallBacks,"y2label",
      			     &SetInterpreter::treatY2Label);
      this->registerCallBack(this->setCallBacks,"y2l",
      			     &SetInterpreter::treatY2Label);
      this->registerCallBack(this->setCallBacks,"t",
      			     &SetInterpreter::treatTerminal);
      this->registerCallBack(this->setCallBacks,"term",
      			     &SetInterpreter::treatTerminal);
      this->registerCallBack(this->setCallBacks,"terminal",
      			     &SetInterpreter::treatTerminal);
      this->registerCallBack(this->setCallBacks,"o",
      			     &SetInterpreter::treatOutput);
      this->registerCallBack(this->setCallBacks,"out",
      			     &SetInterpreter::treatOutput);
      this->registerCallBack(this->setCallBacks,"output",
      			     &SetInterpreter::treatOutput);
      this->registerCallBack(this->setCallBacks,"g",
      			     &SetInterpreter::treatGrid);
      this->registerCallBack(this->setCallBacks,"grid",
      			     &SetInterpreter::treatGrid);
      this->registerCallBack(this->setCallBacks,"xrange",
      			     &SetInterpreter::treatXRange);
      this->registerCallBack(this->setCallBacks,"xr",
      			     &SetInterpreter::treatXRange);
      this->registerCallBack(this->setCallBacks,"x2range",
      			     &SetInterpreter::treatX2Range);
      this->registerCallBack(this->setCallBacks,"x2r",
      			     &SetInterpreter::treatX2Range);
      this->registerCallBack(this->setCallBacks,"yrange",
      			     &SetInterpreter::treatYRange);
      this->registerCallBack(this->setCallBacks,"yr",
      			     &SetInterpreter::treatYRange);
      this->registerCallBack(this->setCallBacks,"y2range",
      			     &SetInterpreter::treatY2Range);
      this->registerCallBack(this->setCallBacks,"y2r",
      			     &SetInterpreter::treatY2Range);
      this->registerCallBack(this->setCallBacks,"logscale",
      			     &SetInterpreter::treatLogScale);
      this->registerCallBack(this->setCallBacks,"arrow",
      			     &SetInterpreter::treatArrow);
      // this->registerCallBack(this->setCallBacks,"label",
      // 			     &SetInterpreter::treatLabel);
      this->registerCallBack(this->setCallBacks,"xtics",
      			     &SetInterpreter::treatXTics);
      this->registerCallBack(this->setCallBacks,"x2tics",
      			     &SetInterpreter::treatX2Tics);
      this->registerCallBack(this->setCallBacks,"ytics",
      			     &SetInterpreter::treatYTics);
      this->registerCallBack(this->setCallBacks,"y2tics",
      			     &SetInterpreter::treatY2Tics);
      this->registerCallBack(this->setCallBacks,"dummy",
      			     &SetInterpreter::treatDummy);
      this->registerCallBack(this->setCallBacks,"title",
      			     &SetInterpreter::treatTitle);
      this->registerCallBack(this->setCallBacks,"key",
      			     &SetInterpreter::treatKey);
      this->registerCallBack(this->setCallBacks,"curve",
      			     &SetInterpreter::treatCurve);
      // this->registerCallBack(this->setCallBacks,"width",
      // 			     &SetInterpreter::treatWidth);
      // this->registerCallBack(this->setCallBacks,"height",
      // 			     &SetInterpreter::treatHeight);
      this->registerCallBack(this->setCallBacks,"theme",
      			     &SetInterpreter::treatTheme);
    } // end of GnuplotInterpreter::SetInterpreter::registerCallBack

    void GnuplotInterpreter::SetInterpreter::eval(const_iterator& p, 
					     const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::eval","",p,pe);
      map<string,MemFuncPtr>::const_iterator ps;
      ps = this->setCallBacks.find(p->value);
      if(ps == this->setCallBacks.end()){
      	string msg("GnuplotInterpreter::eval : ");
      	msg += "unknown option '"+p->value+"'";
      	throw(runtime_error(msg));
      }
      ++p;
      (this->*(ps->second))(p,pe);
    } // end of GnuplotInterpreter::SetInterpreter::eval

    void GnuplotInterpreter::SetInterpreter::treatBorder(const_iterator&,
						   const const_iterator)
    {
      this->g.showBorder(true);
    }

    void GnuplotInterpreter::SetInterpreter::treatNoBorder(const_iterator&,
						   const const_iterator)
    {
      this->g.hideBorder(true);
    }

    void GnuplotInterpreter::SetInterpreter::treatCurve(const_iterator& p,
						   const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatCurve",
				      "expected curve id",p,pe);
      string msg("GnuplotInterpreter::treatCurve : ");
      msg += "unimplemented feature";
      throw(runtime_error(msg));
    }

    void GnuplotInterpreter::SetInterpreter::treatXLabel(const_iterator& p,
						    const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatXLabel",
				      "expected label name",p,pe);
      this->g.setXLabel(this->readQString(p,pe),true);
    } // end of GnuplotInterpreter::SetInterpreter::treatXLabel

    void GnuplotInterpreter::SetInterpreter::treatKey(const_iterator& p,
						 const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatXLabel",
				      "expected key vertical position",p,pe);
      if(p->value=="left"){
	this->g.setKeyHorizontalPosition(Graph::LEFT,false);
	++p;
      } else if(p->value=="right"){
	this->g.setKeyHorizontalPosition(Graph::RIGHT,false);
	++p;
      } else if(p->value=="center"){
	this->g.setKeyHorizontalPosition(Graph::HCENTER,false);
	++p;
      }
      if(p==pe){
	this->g.replot();
	return;
      }
      if(p->value=="top"){
	this->g.setKeyVerticalPosition(Graph::TOP,false);
	++p;
      } else if(p->value=="bottom"){
	this->g.setKeyVerticalPosition(Graph::BOTTOM,false);
	++p;
      } else if(p->value=="center"){
	this->g.setKeyVerticalPosition(Graph::VCENTER,false);
	++p;
      }
      this->g.replot();
    } // end of GnuplotInterpreter::SetInterpreter::treatKey

    void GnuplotInterpreter::SetInterpreter::treatX2Label(const_iterator& p,
						     const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatX2label",
				      "expected label name",p,pe);
      this->g.setX2Label(this->readQString(p,pe),true);
    } // end of GnuplotInterpreter::SetInterpreter::treatX2Label

    void GnuplotInterpreter::SetInterpreter::treatYLabel(const_iterator& p,
						    const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatYlabel",
				      "expected label name",p,pe);
      this->g.setYLabel(this->readQString(p,pe),true);
    } // end of GnuplotInterpreter::SetInterpreter::treatYLabel

    void GnuplotInterpreter::SetInterpreter::treatY2Label(const_iterator& p,
						     const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatY2label",
				      "expected label name",p,pe);
      this->g.setY2Label(this->readQString(p,pe),true);
    } // end of GnuplotInterpreter::SetInterpreter::treatY2Label

    void GnuplotInterpreter::SetInterpreter::treatOutput(const_iterator& p, 
						    const const_iterator pe)
    {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatOutput",
				      "expected output name",p,pe);
      interpreter.setOutput(CxxTokenizer::readString(p,pe));
    } // end of GnuplotInterpreter::SetInterpreter::treatOutput

    void GnuplotInterpreter::SetInterpreter::treatTerminal(const_iterator& p, 
							   const const_iterator pe)
    {
      std::vector<std::string> options;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatTerminal",
				      "expected terminal name",p,pe);
      interpreter.setTerminal(p->value,options);
      while(p!=pe){
	options.push_back(p->value);
	++p;
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatTerminal

    void GnuplotInterpreter::SetInterpreter::treatGrid(const_iterator& p, 
						  const const_iterator pe)
    {
      using namespace std;
      unsigned short grid = this->g.getGrid();
      while(p!=pe){
	if(p->value=="xtics"){
	  grid = grid|Grid::X;
	} else if(p->value=="noxtics"){
	  if(grid&Grid::X){
	    grid = grid^Grid::X;
	  }
	} else if(p->value=="ytics"){
	  grid = grid|Grid::Y;
	} else if(p->value=="noytics"){
	  if(grid&Grid::Y){
	    grid = grid^Grid::Y;
	  }
	} else if(p->value=="x2tics"){
	  grid = grid|Grid::X2;
	} else if(p->value=="nox2tics"){
	  if(grid&Grid::X2){
	    grid = grid^Grid::X2;
	  }
	} else if(p->value=="y2tics"){
	  grid = grid|Grid::Y2;
	} else if(p->value=="noy2tics"){
	  if(grid&Grid::Y2){
	    grid = grid^Grid::Y2;
	  }
	} else {
	  string msg("GnuplotInterpreter::SetInterpreter::treatGrid : ");
	  msg += "unknown grid type '"+p->value+"'";
	  throw(runtime_error(msg));
	}
	++p;
      }
      this->g.setGrid(grid);
      this->g.showGrid();
    } //end of GnuplotInterpreter::SetInterpreter::treatGrid

    void GnuplotInterpreter::SetInterpreter::treatXRange(const_iterator& p,
						    const const_iterator pe)
    {
      using namespace tfel::math::parser;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatXRange",
				      "",p,pe);
      if(p->value=="auto"){
	this->g.unsetXRange();
	++p;
      } else {
	std::shared_ptr<ExternalFunctionManager> functions;
	functions = this->interpreter.getExternalFunctionManager();
	bool  bmin = false;
	bool  bmax = false;
	qreal min;
	qreal max;
	GnuplotInterpreterBase::readRange(bmin,bmax,min,max,functions,p,pe);
	if(bmin||bmax){
	  qreal v0;
	  qreal v1;
	  this->g.getRange(v0,v1,Graph::xaxis);
	  if(bmin){
	    v0 = min;
	  }
	  if(bmax){
	    v1 = max;
	  }
	  this->g.setXRange(v0,v1);
	}
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatXRange

    void GnuplotInterpreter::SetInterpreter::treatX2Range(const_iterator& p,
						     const const_iterator pe)
    {
      using namespace tfel::math::parser;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatXRange",
				      "",p,pe);
      if(p->value=="auto"){
	this->g.unsetX2Range();
	++p;
      } else {
	std::shared_ptr<ExternalFunctionManager> functions;
	functions = this->interpreter.getExternalFunctionManager();
	bool  bmin = false;
	bool  bmax = false;
	qreal min;
	qreal max;
	GnuplotInterpreterBase::readRange(bmin,bmax,min,max,functions,p,pe);
	if(bmin||bmax){
	  qreal v0;
	  qreal v1;
	  this->g.getRange(v0,v1,Graph::x2axis);
	  if(bmin){
	    v0 = min;
	  }
	  if(bmax){
	    v1 = max;
	  }
	  this->g.setX2Range(v0,v1);
	}
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatX2Range

    void GnuplotInterpreter::SetInterpreter::treatYRange(const_iterator& p,
						    const const_iterator pe)
    {
      using namespace tfel::math::parser;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatXRange",
				      "",p,pe);
      if(p->value=="auto"){
	this->g.unsetYRange();
	++p;
      } else {
	std::shared_ptr<ExternalFunctionManager> functions;
	functions = this->interpreter.getExternalFunctionManager();
	bool  bmin = false;
	bool  bmax = false;
	qreal min;
	qreal max;
	GnuplotInterpreterBase::readRange(bmin,bmax,min,max,functions,p,pe);
	if(bmin||bmax){
	  qreal v0;
	  qreal v1;
	  this->g.getRange(v0,v1,Graph::yaxis);
	  if(bmin){
	    v0 = min;
	  }
	  if(bmax){
	    v1 = max;
	  }
	  this->g.setYRange(v0,v1);
	}
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatYRange

    void GnuplotInterpreter::SetInterpreter::treatY2Range(const_iterator& p,
						     const const_iterator pe)
    {
      using namespace tfel::math::parser;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatXRange",
				      "",p,pe);
      if(p->value=="auto"){
	this->g.unsetY2Range();
	++p;
      } else {
	std::shared_ptr<ExternalFunctionManager> functions;
	functions = this->interpreter.getExternalFunctionManager();
	bool  bmin = false;
	bool  bmax = false;
	qreal min;
	qreal max;
	GnuplotInterpreterBase::readRange(bmin,bmax,min,max,functions,p,pe);
	if(bmin||bmax){
	  qreal v0;
	  qreal v1;
	  this->g.getRange(v0,v1,Graph::y2axis);
	  if(bmin){
	    v0 = min;
	  }
	  if(bmax){
	    v1 = max;
	  }
	  this->g.setY2Range(v0,v1);
	}
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatY2Range

    void GnuplotInterpreter::SetInterpreter::treatTitle(const_iterator& p, 
						   const const_iterator pe)
    {
      this->g.setUpperTitle(GnuplotInterpreterBase::readQString(p,pe));
    } //end of GnuplotInterpreter::SetInterpreter::treatTitle

    void GnuplotInterpreter::SetInterpreter::treatDummy(const_iterator& p, 
						   const const_iterator pe)
    {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatDummy",
				      "expected dummy variable name",p,pe);
      interpreter.setDummyVariable(p->value);
      ++p;
    } // end of GnuplotInterpreter::SetInterpreter::treatDummy

    void GnuplotInterpreter::SetInterpreter::treatLogScale(const_iterator& p, 
						      const const_iterator pe)
    {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::"
				      "treatLogScale","",p,pe);
      if(p->value=="x"){
	this->g.setXAxisLogScale();
      } else if(p->value=="y"){
	this->g.setYAxisLogScale();
      } else if(p->value=="x2"){
	this->g.setX2AxisLogScale();
      } else if(p->value=="y2"){
	this->g.setY2AxisLogScale();
      } else {
	throw(std::runtime_error("GnuplotInterpreter::SetInterpreter::treatLogScale: "
				 "unknown axis '"+p->value+"'"));
      }
      ++p;
    } // end of GnuplotInterpreter::SetInterpreter::treatLogScale

    void GnuplotInterpreter::SetInterpreter::treatXTics(const_iterator& p, 
			    const const_iterator pe)
    {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatXTics","",p,pe);
      if(p->value=="auto"){
	this->g.unsetXTics();
      } else {
	this->g.setXTics(this->treatTics(p,pe));
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatXTics

    void GnuplotInterpreter::SetInterpreter::treatX2Tics(const_iterator& p, 
			     const const_iterator pe)
    {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatX2Tics","",p,pe);
      if(p->value=="auto"){
	this->g.unsetX2Tics();
      } else {
	this->g.setX2Tics(this->treatTics(p,pe));
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatX2Tics

    void GnuplotInterpreter::SetInterpreter::treatYTics(const_iterator& p, 
			    const const_iterator pe)
    {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatYTics","",p,pe);
      if(p->value=="auto"){
	this->g.unsetYTics();
      } else {
	this->g.setYTics(this->treatTics(p,pe));
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatYTics

    void GnuplotInterpreter::SetInterpreter::treatY2Tics(const_iterator& p, 
			     const const_iterator pe)
    {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatY2Tics","",p,pe);
      if(p->value=="auto"){
	this->g.unsetY2Tics();
      } else {
	this->g.setY2Tics(this->treatTics(p,pe));
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatY2Tics

    QMap<qreal,QString>
    GnuplotInterpreter::SetInterpreter::treatTics(const_iterator& p, 
						  const const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      QMap<qreal,QString> res;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatSetTics","",p,pe);
      CxxTokenizer::readSpecifiedToken("GnuplotInterpreter::SetInterpreter::treatSetTics","(",p,pe);
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatSetTics","",p,pe);
      if(p->value==")"){
	++p;
	return QMap<qreal,QString>();
      }
      while(p->value!=")"){
	string sev;
	string tic;
	if(p->flag==Token::String){
	  tic = CxxTokenizer::readString(p,pe);
	  if(tic.empty()){
	    string msg("GnuplotInterpreter::SetInterpreter::treatSetTics : ");
	    msg += "empty tic specified";
	    throw(runtime_error(msg));
	  }
	}
	CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatSetTics","",p,pe);
	while((p->value!=",")&&(p->value!=")")){
	  sev += p->value;
	  ++p;
	  CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatSetTics","",p,pe);
	}
	shared_ptr<ExternalFunctionManager> functions;
	functions = this->interpreter.getExternalFunctionManager();
	Evaluator ev(vector<string>(),sev,functions);
	const auto& vars = ev.getVariablesNames();
	if(vars.size()!=0){
	  ostringstream msg;
	  if(vars.size()!=1u){
	    msg << "GnuplotInterpreter::SetInterpreter::treatSetTics : invalid print declaration (unknown variables ";
	    copy(vars.begin(),vars.end(),ostream_iterator<string>(msg," "));
	    msg << ")";
	  } else {
	    msg << "GnuplotInterpreter::SetInterpreter::treatSetTics : invalid print declaration (unknown variable "
		<< vars[0] << ")";
	  }
	  throw(runtime_error(msg.str()));
	}
	const auto value = ev.getValue();
	if(tic.empty()){
	  ostringstream converter;
	  converter << value;
	  res[value] = QString::fromStdString(converter.str());
	} else {
	  res[value] = QString::fromStdString(tic);
	}
	CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatSetTics","",p,pe);
	if(p->value!=")"){
	  CxxTokenizer::readSpecifiedToken("GnuplotInterpreter::SetInterpreter::treatSetTics",",",p,pe);
	  CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatSetTics","",p,pe);
	}
      }
      ++p;
      return res;
    } // end of GnuplotInterpreter::SetInterpreter::treatSetTics

    void GnuplotInterpreter::SetInterpreter::treatTheme(const_iterator& p, 
						   const const_iterator pe)
    {
      using namespace tfel::utilities;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatTheme","",p,pe);
      QString t;
      if(p->flag==Token::String){
	t = QString::fromStdString(p->value.substr(1,p->value.size()-2));
      } else {
	t = QString::fromStdString(p->value);
      }
      ThemeManager& thm = ThemeManager::getThemeManager();
      this->g.setTheme(thm.getTheme(t),false);
    } // end of GnuplotInterpreter::SetInterpreter::treatTheme

    void GnuplotInterpreter::SetInterpreter::treatArrow(const_iterator& p, 
						   const const_iterator pe)
    {
      using namespace std;
      string name;
      ArrowStyle style;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::SetInterpreter::treatArrow","",p,pe);
      if(p->value!="from"){
	name = p->value;
	if(!GnuplotInterpreterBase::isValidIdentifier(p->value)){
	  string msg("GnuplotInterpreter::SetInterpreter::treatArrow : '");
	  msg += p->value+"' is not a valid identifer.";
	  throw(runtime_error(msg));
	}
	++p;
	CxxTokenizer::readSpecifiedToken("GnuplotInterpreter::SetInterpreter::treatArrow","from",p,pe);
      } else {
	++p;
      }
      GraphCoordinates x0;
      this->interpreter.readCoordinates(x0,p,pe,",");
      CxxTokenizer::readSpecifiedToken("GnuplotInterpreter::SetInterpreter::treatArrow",",",p,pe);
      GraphCoordinates y0;
      this->interpreter.readCoordinates(y0,p,pe,"to");
      CxxTokenizer::readSpecifiedToken("GnuplotInterpreter::SetInterpreter::treatArrow ","to",p,pe);
      GraphCoordinates x1;
      this->interpreter.readCoordinates(x1,p,pe,",");
      CxxTokenizer::readSpecifiedToken("GnuplotInterpreter::SetInterpreter::treatArrow",",",p,pe);
      GraphCoordinates y1;
      vector<string> d;
      d.push_back("head");
      d.push_back("heads");
      d.push_back("nohead");
      d.push_back("backhead");
      this->interpreter.readCoordinates(y1,p,pe,d);
      while(p!=pe){
	if(p->value == "head"){
	  style.style = ArrowStyle::HEAD;
	} else if(p->value == "heads"){
	  style.style = ArrowStyle::HEADS;
	} else if(p->value == "nohead"){
	  style.style = ArrowStyle::NOHEAD;
	} else if(p->value == "backhead"){
	  style.style = ArrowStyle::BACKHEAD;
	} else {
	  string msg("GnuplotInterpreter::SetInterpreter::treatArrow : ");
	  msg += "unknown arrow style '"+p->value+"'";
	  throw(runtime_error(msg));
	}
	++p;
      }
      if(name.empty()){
	this->g.addArrow(x0,y0,x1,y1,style);
      } else {
	this->g.addArrow(QString::fromStdString(name),
			 x0,y0,x1,y1,style);
      }
    } // end of GnuplotInterpreter::SetInterpreter::treatArrow

  } // end of namespace plot

} // end of namespace tfel
