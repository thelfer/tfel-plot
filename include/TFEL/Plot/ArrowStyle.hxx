/*!
 * \file   ArrowStyle.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   22 jan 2009
 */

#ifndef LIB_TFEL_PLOT_ARROWSTYLE_HXX_
#define LIB_TFEL_PLOT_ARROWSTYLE_HXX_ 

#include"TFEL/Plot/Config.hxx"

namespace tfel
{

  namespace plot
  {

    struct TFELPLOT_VISIBILITY_EXPORT ArrowStyle
    {
      enum Style{
	NOHEAD,
	HEAD,
	BACKHEAD,
	HEADS
      }; // end of enum Style
      ArrowStyle(const Style = HEAD);
      Style style;
    }; // end of struct ArrowStyle

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_ARROWSTYLE_HXX */
