/*! 
 * \file   GnuplotInterpreter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  12 juin 2012
 */

#ifndef _LIB_TFEL_PLOT_GNUPLOTINTERPRETER_H_
#define _LIB_TFEL_PLOT_GNUPLOTINTERPRETER_H_ 

// we use STL's containers instead of Qt's since we rely on
// CxxTokenizer which works with string's (of course, it results from
// this a lot of QString::fromStdString calls...)
#include<set>
#include<map>
#include<list>
#include<vector>
#include<string>

#include<QtCore/QObject>
#include<QtCore/QString>

#include"TFEL/Plot/Config.hxx"
#include"TFEL/Plot/GnuplotInterpreterBase.hxx"

namespace tfel
{

  namespace math{
    struct Evaluator;
    namespace parser{
      struct ExternalFunction;
    }
  }

  namespace plot
  {

    // forward declaration
    struct GraphCoordinates;

    /*!
     * class in charge of interpreting gnuplot main commands.
     */
    class TFELGNUPLOTINTERPRETER_VISIBILITY_EXPORT GnuplotInterpreter
      : public GnuplotInterpreterBase
    {

      Q_OBJECT

    public:
      
      GnuplotInterpreter(Graph&,
			 QObject * const = nullptr);

      bool parseFile(QString&,
		     const QString&);
      /*!
       * \brief evaluate a string
       * \return true if the string has been evaluated correctly
       * \param[out] emsg: error message in case of failure
       * \param[in]  l:    string to be evaluated
       * \param[in]  b:    if true, emit the errorMsg signal
       */
      bool eval(QString&,
		const QString&,
		const bool = true);

    signals:

      void outputMsg(const QString&);

      void errorMsg(const QString&);

    protected:

      struct PlotInterpreter;
      struct SetInterpreter;
      struct UnSetInterpreter;
      struct ImportInterpreter;
      struct KrigingInterpreter;
      struct FitInterpreter;

      typedef void (GnuplotInterpreter::* MemFuncPtr)(const_iterator&,
						      const const_iterator);

      /*!
       * \param[in] t : terminal type
       * \param[in] o : options
       */
      void
      setTerminal(const std::string&,
		  const std::vector<std::string>&);

      void setOutput(const std::string&);

      void registerCallBacks();

      std::shared_ptr<tfel::math::Evaluator>
      readFunction(const_iterator&, 
		   const const_iterator,
		   const std::string&);

      std::shared_ptr<tfel::math::Evaluator>
      readFunction(const_iterator&, 
		   const const_iterator,
		   const std::vector<std::string>&);
	
      void readCoordinates(GraphCoordinates&,
			   const_iterator&, 
			   const const_iterator,
			   const std::string&);

      void readCoordinates(GraphCoordinates&,
			   const_iterator&, 
			   const const_iterator,
			   const std::vector<std::string>&);
      /*!
       * \brief analyse a new function or variable definition
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       * \param[in]     b1: if true, lock the function definition
       * \param[in]     b2: if true, the dependencies are removed
       *
       * Locking a function `f` means that its definition can't be
       * changed. However, if the function `f` calls another function
       * `g`, the definition of `g` can be changed and that will
       * affect the results returned by `f`.
       *
       * Removing the dependencies means that if `f` depends on
       * another function `g`, the current definition of `g` will be
       * copied locally. Further changes to `g` will no more affect
       * the results returned by `f`.
       *
       * A function defined as `const` is both "locked" and "nodeps".
       */
      void analyseFunctionDefinition(const_iterator&,
				     const const_iterator,
				     const bool,const bool);
      /*!
       * \brief analyse a new function or variable definition
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       * \param[in]     b1: if true, lock the function definition
       * \param[in]     b2: if true, the dependencies are removed
       */
      void addFunction(const std::string&,
		       std::shared_ptr<tfel::math::parser::ExternalFunction>,
		       const bool,const bool);

      void
      readDataFunctionInUsingDeclaration(std::string&,
					 const_iterator&,
					 const const_iterator);

      void
      treatInclude(const_iterator&, 
		   const const_iterator);

      void
      treatImport(const_iterator&, 
		  const const_iterator);

      void
      treatRePlot(const_iterator&, 
		  const const_iterator);

      void
      treatPlot(const_iterator&, 
		const const_iterator);

      void
      treatSet(const_iterator&, 
	       const const_iterator);

      void
      treatUnSet(const_iterator&, 
		 const const_iterator);

      // void
      // treatKriging(const_iterator&, 
      // 		   const const_iterator);

      // void
      // treatFit(const_iterator&, 
      // 	       const const_iterator);

      void
      treatPrint(const_iterator&, 
		 const const_iterator);

      void
      treatQuit(const_iterator&, 
		const const_iterator);

      void
      treatReset(const_iterator&, 
		 const const_iterator);

      void
      treatConst(const_iterator&, 
		 const const_iterator);

      void
      treatLock(const_iterator&, 
		const const_iterator);

      void
      treatNoDeps(const_iterator&, 
		  const const_iterator);

      void
      treatKriging(const_iterator&, 
		   const const_iterator);

      void
      treatFit(const_iterator&, 
	       const const_iterator);

      std::string
      gatherTokenGroup(const_iterator&, 
		       const const_iterator);

      void
      setDummyVariable(const std::string&);

      const std::string&
      getDummyVariable(void) const;

      tfel::math::parser::ExternalFunctionManagerPtr
      getExternalFunctionManager();

      QString currentLine;
      QString previousPlot;
      
      std::string dummyVariable;
      std::string output;
      std::string terminal;
      std::set<std::string> locks;
      std::map<std::string,MemFuncPtr>  callBacks;
      tfel::math::parser::ExternalFunctionManagerPtr functions;

      // std::map<std::string,MemFuncPtr2> plotCallBacks;
      
    }; // end of struct GnuplotInterpreter

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_GNUPLOTINTERPRETER_H */

