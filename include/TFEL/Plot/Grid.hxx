/*!
 * \file   GraphTheme.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   03 jan 2008
 */

#ifndef LIB_TFEL_PLOT_GRID_HXX_
#define LIB_TFEL_PLOT_GRID_HXX_

namespace tfel {

  namespace plot {

    struct Grid {
      static constexpr const unsigned short None = 0;
      static constexpr const unsigned short X = 1 << 1;
      static constexpr const unsigned short X2 = 1 << 2;
      static constexpr const unsigned short Y = 1 << 3;
      static constexpr const unsigned short Y2 = 1 << 4;
    };  // end of struct Grid

  }  // end of namespace plot

}  // end of namespace tfel

#endif /*LIB_TFEL_PLOT_GRID_HXX_ */
