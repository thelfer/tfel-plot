/*! 
 * \file  GnuplotUnSetInterpreter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#ifndef LIB_TFEL_PLOT_GNUPLOTUNSETINTERPRETER_H_
#define LIB_TFEL_PLOT_GNUPLOTUNSETINTERPRETER_H_ 

#include"TFEL/Plot/GnuplotInterpreter.hxx"

namespace tfel
{

  namespace plot
  {

    struct GnuplotInterpreter::UnSetInterpreter
      : GnuplotInterpreterBase
    {

      UnSetInterpreter(Graph&);

      void eval(const_iterator&, 
		const const_iterator);

    protected:

      void registerCallBacks();

      typedef void (UnSetInterpreter::* MemFuncPtr)(const_iterator&,
						    const const_iterator);

      std::map<std::string,MemFuncPtr> callBacks;

      void treatLabel(const_iterator&, 
		      const const_iterator);

      void treatLogScale(const_iterator&, 
			 const const_iterator);

      void treatXLabel(const_iterator&, 
		       const const_iterator);

      void treatX2Label(const_iterator&, 
			const const_iterator);

      void treatYLabel(const_iterator&, 
		       const const_iterator);

      void treatY2Label(const_iterator&, 
			const const_iterator);

      void treatXTics(const_iterator&, 
		      const const_iterator);

      void treatX2Tics(const_iterator&, 
		       const const_iterator);

      void treatYTics(const_iterator&, 
		      const const_iterator);

      void treatY2Tics(const_iterator&, 
		       const const_iterator);

      void treatXRange(const_iterator&, 
		       const const_iterator);

      void treatX2Range(const_iterator&, 
			const const_iterator);

      void treatYRange(const_iterator&, 
		       const const_iterator);

      void treatY2Range(const_iterator&, 
			const const_iterator);

      void treatGrid(const_iterator&, 
		     const const_iterator);

    };

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GNUPLOTUNSETINTERPRETER_H */

