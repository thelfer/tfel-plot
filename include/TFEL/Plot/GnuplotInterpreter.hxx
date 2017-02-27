/*! 
 * \file   GnuplotInterpreter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  12 juin 2012
 */

#ifndef LIB_TFEL_PLOT_GNUPLOTINTERPRETER_H_
#define LIB_TFEL_PLOT_GNUPLOTINTERPRETER_H_ 

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
      bool parseString(QString&,
		       const QString&,
		       const bool = true);
      /*!
       * \brief evaluate a string
       * \return the result of the evaluation.
       * \param[in] l: string to be evaluated
       */
      double eval(const QString&);

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
       * \brief evaluate the next group 
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      double eval(const_iterator&,
		  const const_iterator);
      /*!
       * \param[in] t : terminal type
       * \param[in] o : options
       */
      void setTerminal(const std::string&,
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

      void readDataFunctionInUsingDeclaration(std::string&,
					      const_iterator&,
					      const const_iterator);
      /*!
       * \brief treat the `include` keyword
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatInclude(const_iterator&, 
			const const_iterator);
      /*!
       * \brief treat the `import` keyword. The treatment of this
       * keyword is in fact handled by the `ImportInterpreter` class.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatImport(const_iterator&, 
		       const const_iterator);
      /*!
       * \brief treat the `replot` keyword
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatRePlot(const_iterator&, 
		       const const_iterator);
      /*!
       * \brief treat the `plot` keyword. The treatment of this
       * keyword is in fact handled by the `PlotInterpreter` class.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatPlot(const_iterator&, 
		     const const_iterator);
      /*!
       * \brief treat the `set` keyword. The treatment of this
       * keyword is in fact handled by the `SetInterpreter` class.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatSet(const_iterator&, 
		    const const_iterator);
      /*!
       * \brief treat the `unset` keyword. The treatment of this
       * keyword is in fact handled by the `UnSetInterpreter` class.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatUnSet(const_iterator&, 
		      const const_iterator);
      /*!
       * \brief treat the `print` keyword.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatPrint(const_iterator&, 
		      const const_iterator);
      /*!
       * \brief treat the `quit` keyword.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatQuit(const_iterator&, 
		     const const_iterator);
      /*!
       * \brief treat the `reset` keyword.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatReset(const_iterator&, 
		      const const_iterator);
      /*!
       * \brief treat the `const` keyword.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatConst(const_iterator&, 
		      const const_iterator);
      /*!
       * \brief treat the `lock` keyword.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatLock(const_iterator&, 
		     const const_iterator);
      /*!
       * \brief treat the `nodeps` keyword.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatNoDeps(const_iterator&, 
		       const const_iterator);
      /*!
       * \brief treat the `kriging` keyword. The treatment of this
       * keyword is in fact handled by the `KrigingInterpreter` class.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatKriging(const_iterator&, 
			const const_iterator);
      /*!
       * \brief treat the `fit` keyword. The treatment of this
       * keyword is in fact handled by the `FitInterpreter` class.
       * \param[in,out] p:  iterator to the current position
       * \param[in]     pe: iterator past the end of line
       */
      void treatFit(const_iterator&, 
		    const const_iterator);

      std::string
	gatherTokenGroup(const_iterator&, 
			 const const_iterator);

      void setDummyVariable(const std::string&);

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

#endif /* LIB_TFEL_PLOT_GNUPLOTINTERPRETER_H */

