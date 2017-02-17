/*!
 * \file   GraphCoordinates.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jan 2009
 */

#ifndef _LIB_TFEL_GRAPHCOORDINATES_HXX_
#define _LIB_TFEL_GRAPHCOORDINATES_HXX_ 

#include"TFEL/Plot/Config.hxx"
#include"TFEL/Math/Evaluator.hxx"

namespace tfel
{

  namespace plot
  {

    struct TFELPLOT_VISIBILITY_EXPORT GraphCoordinates
    {
      enum CoordinateType{
	FIRST,
	SECOND,
	GRAPH,
	SCREEN,
	CHARACTER
      }; // end of enum CoordinateType
      GraphCoordinates();
      GraphCoordinates(CoordinateType,
		       const std::shared_ptr<tfel::math::Evaluator>);
      CoordinateType coordinateType;
      std::shared_ptr<tfel::math::Evaluator> c;
    }; // end of GraphCoordinates

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_GRAPHCOORDINATES_HXX_ */
