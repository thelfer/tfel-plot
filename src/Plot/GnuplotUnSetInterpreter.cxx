/*! 
 * \file  GnuplotUnSetInterpreter.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/GnuplotUnSetInterpreter.hxx"

namespace tfel
{

  namespace plot
  {

    GnuplotInterpreter::UnSetInterpreter::UnSetInterpreter(Graph& graph)
      : GnuplotInterpreterBase(graph)
    {
      this->registerCallBacks();
    } // end of GnuplotInterpreter::UnSetInterpreter::UnSetInterpreter

    void GnuplotInterpreter::UnSetInterpreter::registerCallBacks()
    {
      // this->registerCallBack(this->unsetCallBacks,"width",
      // 			     &GnuplotInterpreter::treatWidth);
      // this->registerCallBack(this->unsetCallBacks,"height",
      // 			     &GnuplotInterpreter::treatHeight);
      this->registerCallBack(this->callBacks,"logscale",
      			     &UnSetInterpreter::treatLogScale);
      this->registerCallBack(this->callBacks,"label",
      			     &UnSetInterpreter::treatLabel);
      this->registerCallBack(this->callBacks,"xrange",
      			     &UnSetInterpreter::treatXRange);
      this->registerCallBack(this->callBacks,"x2range",
      			     &UnSetInterpreter::treatX2Range);
      this->registerCallBack(this->callBacks,"yrange",
      			     &UnSetInterpreter::treatYRange);
      this->registerCallBack(this->callBacks,"y2range",
      			     &UnSetInterpreter::treatY2Range);
      this->registerCallBack(this->callBacks,"xtics",
      			     &UnSetInterpreter::treatXTics);
      this->registerCallBack(this->callBacks,"x2tics",
      			     &UnSetInterpreter::treatX2Tics);
      this->registerCallBack(this->callBacks,"ytics",
      			     &UnSetInterpreter::treatYTics);
      this->registerCallBack(this->callBacks,"y2tics",
      			     &UnSetInterpreter::treatY2Tics);
      this->registerCallBack(this->callBacks,"xlabel",
      			     &UnSetInterpreter::treatXLabel);
      this->registerCallBack(this->callBacks,"x2label",
      			     &UnSetInterpreter::treatX2Label);
      this->registerCallBack(this->callBacks,"ylabel",
      			     &UnSetInterpreter::treatYLabel);
      this->registerCallBack(this->callBacks,"y2label",
      			     &UnSetInterpreter::treatY2Label);
      this->registerCallBack(this->callBacks,"grid",
      			     &UnSetInterpreter::treatGrid);
    } // end of UnSetInterpreter::UnSetInterpreter::registerCallBack

    void GnuplotInterpreter::UnSetInterpreter::treatLabel(const_iterator& p, 
							  const const_iterator pe)
    {
      using namespace std;
      if(p==pe){
	this->g.removeLabels();
      } else {
	if(!this->isValidIdentifier(p->value)){
	  string msg("GnuplotInterpreter::UnSetInterpreter::treatLabel : '");
	  msg += p->value+"' is not a valid identifer.";
	  throw(runtime_error(msg));
	}
	this->g.removeLabel(QString::fromStdString(p->value));
	++p;
      }
    } // end of GnuplotInterpreter::UnSetInterpreter::treatLabel

    void GnuplotInterpreter::UnSetInterpreter::treatLogScale(const_iterator& p, 
							     const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::UnSetInterpreter::treatLogScale","",p,pe);
      if(p->value=="x"){
	this->g.unsetXAxisLogScale();
      } else if(p->value=="y"){
	this->g.unsetYAxisLogScale();
      } else if(p->value=="x2"){
	this->g.unsetX2AxisLogScale();
      } else if(p->value=="y2"){
	this->g.unsetY2AxisLogScale();
      } else {
	string msg("GnuplotInterpreter::UnSetInterpreter::treatLogScale : ");
	msg += "unknown axis '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
    } // end of GnuplotInterpreter::UnSetInterpreter::treatLogScale

    void GnuplotInterpreter::UnSetInterpreter::eval(const_iterator& p, 
						    const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::UnSetInterpreter::eval","",p,pe);
      map<string,MemFuncPtr>::const_iterator ps;
      ps = this->callBacks.find(p->value);
      if(ps == this->callBacks.end()){
      	string msg("GnuplotInterpreter::eval : ");
      	msg += "unknown option '"+p->value+"'";
      	throw(runtime_error(msg));
      }
      ++p;
      (this->*(ps->second))(p,pe);
    } // end of GnuplotInterpreter::UnSetInterpreter::eval

    void GnuplotInterpreter::UnSetInterpreter::treatXRange(const_iterator&,
							   const const_iterator)
    {
      this->g.unsetXRange();
    } // end of GnuplotInterpreter::UnSetInterpreter::treatXRange

    void GnuplotInterpreter::UnSetInterpreter::treatX2Range(const_iterator&,
							    const const_iterator)
    {
      this->g.unsetX2Range();
    } // end of GnuplotInterpreter::UnSetInterpreter::treatXRange

    void GnuplotInterpreter::UnSetInterpreter::treatYRange(const_iterator&,
							   const const_iterator)
    {
      this->g.unsetYRange();
    } // end of GnuplotInterpreter::UnSetInterpreter::treatYRange

    void GnuplotInterpreter::UnSetInterpreter::treatY2Range(const_iterator&,
							    const const_iterator)
    {
      this->g.unsetY2Range();
    } // end of GnuplotInterpreter::UnSetInterpreter::treatY2Range

    void GnuplotInterpreter::UnSetInterpreter::treatXLabel(const_iterator&, 
							   const const_iterator)
    {
      this->g.setXLabel("");
    } // end of GnuplotInterpreter::UnSetInterpreter::treatXLabel

    void GnuplotInterpreter::UnSetInterpreter::treatX2Label(const_iterator&, 
							    const const_iterator)
    {
      this->g.setX2Label("");
    } // end of GnuplotInterpreter::UnSetInterpreter::treatX2Label

    void GnuplotInterpreter::UnSetInterpreter::treatYLabel(const_iterator&, 
							   const const_iterator)
    {
      this->g.setYLabel("");
    } // end of GnuplotInterpreter::UnSetInterpreter::treatYLabel

    void GnuplotInterpreter::UnSetInterpreter::treatY2Label(const_iterator&, 
							    const const_iterator)
    {
      this->g.setY2Label("");
    } // end of GnuplotInterpreter::UnSetInterpreter::treatY2Label

    void GnuplotInterpreter::UnSetInterpreter::treatGrid(const_iterator&, 
							 const const_iterator)
    {
      this->g.showGrid(false);
    } // end of GnuplotInterpreter::UnSetInterpreter::treatY2Label

    void GnuplotInterpreter::UnSetInterpreter::treatXTics(const_iterator&, 
							  const const_iterator)
    {
      this->g.unsetXTics();
    } // end of GnuplotInterpreter::UnSetInterpreter::treatXTics

    void GnuplotInterpreter::UnSetInterpreter::treatX2Tics(const_iterator&, 
							   const const_iterator)
    {
      this->g.unsetX2Tics();
    } // end of GnuplotInterpreter::UnSetInterpreter::treatXTics

    void GnuplotInterpreter::UnSetInterpreter::treatYTics(const_iterator&, 
							  const const_iterator)
    {
      this->g.unsetYTics();
    } // end of GnuplotInterpreter::UnSetInterpreter::treatXTics

    void GnuplotInterpreter::UnSetInterpreter::treatY2Tics(const_iterator&, 
							   const const_iterator)
    {
      this->g.unsetY2Tics();
    } // end of GnuplotInterpreter::UnSetInterpreter::treatXTics

  } // end of namespace plot

} // end of namespace tfel
