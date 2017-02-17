/*! 
 * \file  GnuplotKrigingInterpreter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 juin 2012
 */

#ifndef _LIB_TFEL_PLOT_GNUPLOTKRIGINGINTERPRETER_H_
#define _LIB_TFEL_PLOT_GNUPLOTKRIGINGINTERPRETER_H_ 

#include"TFEL/Plot/GnuplotInterpreter.hxx"

namespace tfel
{

  namespace plot
  {

    struct GnuplotInterpreter::KrigingInterpreter
      : GnuplotInterpreterBase
    {

      KrigingInterpreter(GnuplotInterpreter&,
			 Graph& g);

      void eval(const_iterator&, 
		const const_iterator);

    protected:

      GnuplotInterpreter& interpreter;

    };

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_GNUPLOTKRIGINGINTERPRETER_H */

