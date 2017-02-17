/*!
 * \file   GraphCoordinates.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 jan 2009
 */

#include"TFEL/Plot/GraphCoordinates.hxx"

namespace tfel
{

  namespace plot
  {

    GraphCoordinates::GraphCoordinates() = default;

    GraphCoordinates::GraphCoordinates(const CoordinateType ct_,
				       const std::shared_ptr<tfel::math::Evaluator> c_)
      : coordinateType(ct_),
	c(c_)
    {} // end of GraphCoordinates::GraphCoordinates

  } // end of namespace plot

} // end of namespace tfel
