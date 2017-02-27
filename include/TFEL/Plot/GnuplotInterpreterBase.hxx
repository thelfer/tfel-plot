/*! 
 * \file  GnuplotInterpreterBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#ifndef LIB_TFEL_PLOT_GNUPLOTINTERPRETERBASE_H_
#define LIB_TFEL_PLOT_GNUPLOTINTERPRETERBASE_H_ 

#include<QtCore/QObject>
#include<QtCore/QString>
#include<QtCore/QVector>

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include"TFEL/Plot/Config.hxx"

namespace tfel
{

  namespace plot
  {

    // forward declaration
    struct Graph;

    // forward declaration
    struct TextDataReader;

    /*!
     * base class for classes in charge of interpreting gnuplot
     * commands
     */
    class TFELGNUPLOTINTERPRETER_VISIBILITY_EXPORT GnuplotInterpreterBase
      : public QObject
    {
      
      Q_OBJECT

    protected:

      //! a simple alias
      using Token = tfel::utilities::Token;
      //! a simple alias
      using CxxTokenizer = tfel::utilities::CxxTokenizer;
      //! a simple alias
      using TokensContainer = CxxTokenizer::TokensContainer;
      //! a simple alias
      using const_iterator = TokensContainer::const_iterator;
      /*!
       * \param[in] g : graph that will be affected
       * by the interpreter
       */
      GnuplotInterpreterBase(Graph&,
			     QObject *const = nullptr);

      template<typename T>
      void registerCallBack(std::map<std::string,T>&,
			    const std::string&,
			    const T);

      static void
      throwKeyAlreadyRegistredCallBack(const std::string&);

      static QString
      readQString(const_iterator&,
		  const const_iterator);

      static double
      readDouble(const std::string&,
		 const unsigned short);

      static bool
      isUnsignedInteger(const std::string&);

      static unsigned short
      convertToUnsignedShort(const std::string&);

      static void
      readVariableList(QVector<std::string>&,
		       const_iterator&, 
		       const const_iterator,
		       const bool = true);

      static QVector<std::string>
      readVariableList(const_iterator&, 
		       const const_iterator,
		       const bool = true);

      static bool
      isValidIdentifier(const std::string&);

      static std::string
      readNextGroup(const_iterator&,
		    const const_iterator);

      static void
      readRange(bool&,
		bool&,
		double&,
		double&,
		tfel::math::parser::ExternalFunctionManagerPtr&,
		const_iterator&, 
		const const_iterator);

      static std::string
      readUntil(const_iterator&,
		const const_iterator,
		const std::string&);

      static void
      readDataFunctionInUsingDeclaration(std::string&,
					 const_iterator&,
					 const const_iterator);

      static void
      getData(QVector<double>&,
	      tfel::math::parser::ExternalFunctionManagerPtr,
	      const TextDataReader&,
	      const_iterator&,
	      const const_iterator);

      static QString
      getData(QVector<double>&,
	      tfel::math::parser::ExternalFunctionManagerPtr,
	      const TextDataReader&,
	      const std::string&);      

      //! graph that will be affected by the interpreter
      Graph& g;

    }; // end of struct GnuplotInterpreterBase

  } // end of namespace plot

} // end of namespace tfel

#include"TFEL/Plot/GnuplotInterpreterBase.ixx"

#endif /* LIB_TFEL_PLOT_GNUPLOTINTERPRETERBASE_H */
