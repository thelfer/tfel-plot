/*!
 * \file   Config.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 févr. 2016
 */

#ifndef _LIB_TFEL_PLOT_CONFIG_HXX_
#define _LIB_TFEL_PLOT_CONFIG_HXX_

#include"TFEL/Config/TFELConfig.hxx"

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELPlot_EXPORTS
#    define TFELPLOT_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#  ifndef TFEL_STATIC_BUILD
#    define TFELPLOT_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  else 
#    define TFELPLOT_VISIBILITY_EXPORT
#  endif
#  endif
#else
#  define TFELPLOT_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELGnuplotInterpreter_EXPORTS
#    define TFELGNUPLOTINTERPRETER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#  ifndef TFEL_STATIC_BUILD
#    define TFELGNUPLOTINTERPRETER_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  else 
#    define TFELGNUPLOTINTERPRETER_VISIBILITY_EXPORT
#  endif
#  endif
#else
#  define TFELGNUPLOTINTERPRETER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#endif /* _LIB_TFEL_PLOT_CONFIG_HXX_ */
