/*! 
 * \file  GnuplotFitInterpreter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 juin 2012
 */

#ifndef _LIB_TFEL_PLOT_GNUPLOTFITINTERPRETER_H_
#define _LIB_TFEL_PLOT_GNUPLOTFITINTERPRETER_H_ 

#include"TFEL/Plot/GnuplotInterpreter.hxx"

namespace tfel
{

  namespace plot
  {

    struct GnuplotInterpreter::FitInterpreter
      : public GnuplotInterpreterBase
    {

      FitInterpreter(GnuplotInterpreter&,
		     Graph&);

      void eval(const_iterator&, 
		const const_iterator);

    protected:

      GnuplotInterpreter& interpreter;

    }; // end of struct GnuplotInterpreter::FitInterpreter

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_GNUPLOTFITINTERPRETER_H */

