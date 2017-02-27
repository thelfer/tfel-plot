/*! 
 * \file  GnuplotImportInterpreter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#ifndef LIB_TFEL_PLOT_GNUPLOTIMPORTINTERPRETER_H_
#define LIB_TFEL_PLOT_GNUPLOTIMPORTINTERPRETER_H_ 

#include"TFEL/Plot/GnuplotInterpreter.hxx"

namespace tfel
{

  namespace plot
  {

    struct GnuplotInterpreter::ImportInterpreter
      : GnuplotInterpreterBase
    {

      ImportInterpreter(GnuplotInterpreter&,
			Graph&);

      void treatImport(const_iterator&,
		       const const_iterator,
		       const bool);

    protected:

      struct ImportOptions;

      void importCFunction(const std::string&,
			   const std::string&,
			   const unsigned short,
			   const bool);

      void importCastemFunction(const std::string&,
				const std::string&,
				const unsigned short,
				const bool);
      
      void importFunction(const_iterator&, 
			  const const_iterator,
			  const ImportOptions,
			  const bool);

      GnuplotInterpreter& interpreter;

    };

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GNUPLOTIMPORTINTERPRETER_H */

