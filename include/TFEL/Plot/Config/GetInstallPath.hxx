/*!
 * \file   GetInstallPath.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   01 mars 2017
 */

#ifndef LIB_TFEL_PATH_GETINSTALLPATH_HXX
#define LIB_TFEL_PATH_GETINSTALLPATH_HXX

#include<string>
#include"TFEL/Plot/Config.hxx"

namespace tfel{

  namespace plot{

    TFELPLOT_VISIBILITY_EXPORT std::string
    getInstallPath();

  } // end of namespace plot
    
} // end of namespace tfel

#endif /* LIB_TFEL_PATH_GETINSTALLPATH_HXX */
