/*! 
 * \file   GnuplotInterpreterBase.ixx
 * \brief
 * \author Helfer Thomas
 * \brief  12 juin 2012
 */

#include<stdexcept>

#ifndef _LIB_TFEL_PLOT_GNUPLOTINTERPRETERBASE_IXX_
#define _LIB_TFEL_PLOT_GNUPLOTINTERPRETERBASE_IXX_ 


namespace tfel
{

  namespace plot
  {

    template<typename T>
    void
    GnuplotInterpreterBase::registerCallBack(std::map<std::string,T>& m,
					     const std::string& name,
					     const T f)
    {
      using namespace std;
      typedef typename  std::map<string,T>::value_type MVType;
      if(!m.insert(MVType(name,f)).second){
	GnuplotInterpreterBase::throwKeyAlreadyRegistredCallBack(name);
      }
    } // end of GnuplotInterpreterBase::registerCallBack

  } // end of namespace plot

} // end of namespace tfel


#endif /* _LIB_TFEL_PLOT_GNUPLOTINTERPRETERBASE_IXX */
