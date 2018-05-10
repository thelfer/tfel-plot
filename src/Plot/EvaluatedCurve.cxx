/*!
 * \file   EvaluatedCurve.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2008
 */

#include<cmath>
#include"TFEL/Plot/EvaluatedCurve.hxx"

namespace tfel{

  namespace plot{

    EvaluatedCurve::EvaluatedCurve(std::shared_ptr<tfel::math::Evaluator>& f,
				   const QString& v)
      : ev(f),
	hasSamples(false),
	isConstant(false)
    {
      using namespace std;
      const string& var = v.toStdString();
      const vector<string>& names = this->ev->getVariablesNames();
      if((!names.empty())&&(names.size()!=1u)){
	vector<string>::const_iterator p;
	string msg("EvaluatedCurve::EvaluatedCurve : ");
	msg += "function declares more than one variable ";
	msg += "(read : ";
	p = names.begin();
	while(p != names.end()){
	  msg += *p;
	  ++p;
	  if(p!=names.end()){
	    msg += ").";
	  } else {
	    msg += " ";
	  }
	}
      }
      if(names.size()==1u){
	if(names[0]!=var){
	  string msg("EvaluatedCurve::EvaluatedCurve : ");
	  msg += "function declares another variable than '"+var+"' ";
	  msg += "(read : " +names[0]+")";
	}
      } else {
	this->isConstant = true;
      }
    } // end of EvaluatedCurve::EvaluatedCurve

    bool EvaluatedCurve::hasRange() const
    {
      return false;
    } // end of EvaluatedCurve::hasRange

    qreal EvaluatedCurve::minRange() const
    {
      using namespace std;
      return -numeric_limits<qreal>::max();
    } // end of EvaluatedCurve::

    qreal EvaluatedCurve::maxRange() const
    {
      using namespace std;
      return numeric_limits<qreal>::max();
    } // end of EvaluatedCurve::maxRange

    EvaluatedCurve::~EvaluatedCurve() = default;

    void EvaluatedCurve::getValues(std::vector<Point>& points,
                                   const qreal xmin,
                                   const qreal xmax,
                                   const unsigned short s) {
      using namespace std;
      const qreal dx = (xmax-xmin)/s;
      points.clear();
      if(this->isConstant){
	for(qreal x = xmin;x<xmax+0.5*dx;x+=dx){
	  Point p;
	  p.x = x;
	  p.y = this->ev->getValue();
	  points.push_back(p);
	}
      } else {
	for(qreal x = xmin;x<xmax+0.5*dx;x+=dx){
	  Point p;
	  this->ev->setVariableValue(std::vector<double>::size_type{0},x);
	  p.x = x;
	  try {
	    p.y = this->ev->getValue();
	  } catch(runtime_error& e){
	    p.y = nan(e.what());
	  }
	  points.push_back(p);
	}
      }
    } // end of EvaluatedCurve::getValues

    bool EvaluatedCurve::hasSpecifiedNumberOfSamples() const
    {
      return this->hasSamples;
    } // end of EvaluatedCurve::hasSpecifiedNumberOfSamples

    void
    EvaluatedCurve::setNumberOfSamples(const unsigned short f,
				       const bool b)
    {
      this->hasSamples = true;
      this->samples = f;
      if(b){
	emit updated(this);
      }
    } // end of EvaluatedCurve::setNumberOfSamples

    unsigned short 
    EvaluatedCurve::getNumberOfSamples() const
    {
      return this->samples;
    } // end of EvaluatedCurve::getNumberOfSamples

  } // end of namespace plot

} // end of namespace tfel
