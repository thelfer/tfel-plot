/*!
 * \file  GnuplotPlotInterpreter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#ifndef LIB_TFEL_PLOT_GNUPLOTPLOTINTERPRETER_H_
#define LIB_TFEL_PLOT_GNUPLOTPLOTINTERPRETER_H_

#include "TFEL/Plot/GnuplotInterpreter.hxx"

namespace tfel {

  namespace utilities {
    struct TextData;
  }  // end of namespace utilities

  namespace plot {

    struct GnuplotInterpreter::PlotInterpreter : GnuplotInterpreterBase {
      PlotInterpreter(GnuplotInterpreter&, Graph&);

      void eval(const_iterator&, const const_iterator);

     protected:
      struct CurveOptions;

      void registerCallBacks();

      typedef void (PlotInterpreter::*MemFuncPtr)(CurveOptions&,
                                                  const_iterator&,
                                                  const const_iterator);

      void treatDataPlot(const_iterator&, const const_iterator);

      void treatFunctionPlot(const_iterator&, const const_iterator);

      CurveOptions treatPlotOptions(const_iterator&, const const_iterator);

      void applyCurveOptions(std::shared_ptr<Curve>, const CurveOptions&);

      void treatTitle(CurveOptions&, const_iterator&, const const_iterator);

      void treatWith(CurveOptions&, const_iterator&, const const_iterator);

      void treatUsing(CurveOptions&, const_iterator&, const const_iterator);

      void treatAxes(CurveOptions&, const_iterator&, const const_iterator);

      void treatLineType(CurveOptions&, const_iterator&, const const_iterator);

      void treatLineColor(CurveOptions&, const_iterator&, const const_iterator);

      void treatLineWidth(CurveOptions&, const_iterator&, const const_iterator);

      void treatNoTitle(CurveOptions&, const_iterator&, const const_iterator);

      std::map<std::string, MemFuncPtr> plotCallBacks;
      //! reference to the interpreter
      GnuplotInterpreter& interpreter;
    };

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GNUPLOTPLOTINTERPRETER_H */
