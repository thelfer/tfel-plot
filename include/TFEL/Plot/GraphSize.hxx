/*!
 * \file   GraphSize.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef LIB_TFEL_PLOT_GRAPHSIZE_HXX_
#define LIB_TFEL_PLOT_GRAPHSIZE_HXX_ 

namespace tfel{

  namespace plot
  {

    struct GraphSize
    {
      qreal width;  // graph width
      qreal height; // graph width
      qreal xh0;    // left  graph abscissa
      qreal xh1;    // right graph abscissa  
      qreal yh0;    // lower graph abscissa
      qreal yh1;    // upper graph abscissa
    }; // end of struct GraphSize

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHSIZE_HXX */

