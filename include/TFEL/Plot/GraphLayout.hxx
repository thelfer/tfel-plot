/*!
 * \file   GraphLayout.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef LIB_TFEL_PLOT_GRAPHLAYOUT_HXX_
#define LIB_TFEL_PLOT_GRAPHLAYOUT_HXX_

namespace tfel {

  namespace plot {

    struct GraphLayout {
      qreal ax;
      qreal bx;
      qreal ay;
      qreal by;
      qreal ax2;
      qreal bx2;
      qreal ay2;
      qreal by2;
      qreal ml;   // left margin size
      qreal mr;   // right margin size
      qreal mu;   // up margin size
      qreal md;   // down margin size
      qreal ttl;  // left title size
      qreal ttr;  // right title size
      qreal ttu;  // up title size
      qreal ttd;  // down title size
      qreal ll;   // left label size
      qreal lr;   // right label size
      qreal lu;   // up label size
      qreal ld;   // down label size
      qreal tl;   // left tics size
      qreal tr;   // right tics size
      qreal tu;   // up tics size
      qreal td;   // down tics size
      qreal gl;   // left  graph margin size
      qreal gr;   // right graph margin size
      qreal gu;   // upper graph margin size
      qreal gd;   // down  graph margin size
      qreal bl;   // left border size
      qreal br;   // right border size
      qreal bu;   // upper border size
      qreal bd;   // down border size
    };            // end of struct GraphLayout

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHLAYOUT_HXX */
