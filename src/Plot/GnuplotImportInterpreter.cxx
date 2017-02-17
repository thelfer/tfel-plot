/*! 
 * \file  GnuplotImportInterpreter.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#include<sstream>

#include"TFEL/Math/Parser/ExternalCFunction.hxx"
#include"TFEL/Math/Parser/ExternalCastemFunction.hxx"

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"TFEL/Plot/GnuplotImportInterpreter.hxx"

namespace tfel
{

  namespace plot
  {

    struct GnuplotInterpreter::ImportInterpreter::ImportOptions
    {
      enum ImportType{
	Function,
	Data,
	Default
      };
      enum FunctionType{
	C,
	Castem,
	FDefault
      };
      ImportOptions() = default;
      ImportType type = Default;
      FunctionType functionType = FDefault;
      int numberOfVariables = -1;
    }; // end of GnuplotInterpreter::ImportOptions

    GnuplotInterpreter::ImportInterpreter::ImportInterpreter(GnuplotInterpreter& i,
							     Graph& graph)
      : GnuplotInterpreterBase(graph),
	interpreter(i)
    {} // end of GnuplotInterpreter::ImportInterpreter::ImportInterpreter

    void
    GnuplotInterpreter::ImportInterpreter::treatImport(const_iterator& p,
						       const const_iterator pe,
						       const bool b)
    {
      auto throw_if = [](const bool c,const std::string& m){
	if(c){throw(std::runtime_error("GnuplotInterpreter::ImportInterpreter::treatImport: "+m));}
      };
      ImportOptions options;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::ImportInterpreter::treatImport : ",
				      "expected function name",p,pe);
      if(p->value=="<"){
	++p;
	throw_if(p==pe,"unexpected end of line");
	// import options
	while(p->value!=">"){
	  if(p->value=="function"){
	    throw_if(options.type!=ImportOptions::Default,
		     "import type has already been specified");
	    options.type=ImportOptions::Function;
	  } else if (p->value=="c"){
	    throw_if(!((options.type==ImportOptions::Default)||
		       (options.type==ImportOptions::Function)),
		     "import type is neither 'Default' nor 'Function', "
		     "which is required for the 'c' option");
	    throw_if(options.functionType!=ImportOptions::FDefault,
		     "a function type has already been specified");
	    options.type=ImportOptions::Function;
	    options.functionType=ImportOptions::C;
	  } else if (p->value=="castem"){
	    throw_if(!((options.type==ImportOptions::Default)||
		       (options.type==ImportOptions::Function)),
		     "import type is neither 'Default' nor 'Function', "
		     "which is required for the 'castem' option");
	    throw_if(options.functionType!=ImportOptions::FDefault,
		     "a function type has already been specified");
	    options.type=ImportOptions::Function;
	    options.functionType=ImportOptions::Castem;
	  } else if(p->value=="data"){
	    throw_if(options.functionType!=ImportOptions::FDefault,
		     "a function type has been specified, "
		     "which is inconsistent with 'data' options");
	    throw_if(options.type!=ImportOptions::Default,
		     "import type has already been specified");
	    options.type=ImportOptions::Data;
	  } else {
	    // must be the number of variables
	    unsigned short nbr;
	    try{
	      nbr = this->convertToUnsignedShort(p->value);
	    } catch(std::runtime_error&){
	      throw_if(true,"unknown option '"+p->value+"'");
	    }
	    throw_if(options.numberOfVariables!=-1,
		     "the number of variables has alread been specified");
	    options.numberOfVariables = static_cast<int>(nbr);
	  }
	  ++p;
	  throw_if(p==pe,"unexpected end of line");
	  throw_if((p->value!=">")&&(p->value!=","),
		   "unexpected token '"+p->value+"', expected ',' or '>'");
	  if(p->value==","){
	    ++p;
	  }
	}
	++p;	
      }
      if((options.type==ImportOptions::Default)||
	 (options.type==ImportOptions::Function)){
	this->importFunction(p,pe,options,b);
      } else {
	throw_if(true,"only function import is supported");
      }
    } // end of GnuplotInterpreter::ImportInterpreter::treatImport

    void
    GnuplotInterpreter::ImportInterpreter::importCastemFunction(const std::string& function,
								const std::string& library,
								const unsigned short varNumber,
								const bool b)
    {
      auto& elm = tfel::system::ExternalLibraryManager::getExternalLibraryManager();
      const auto nb = elm.getCastemFunctionNumberOfVariables(library,function);
      if(nb!=varNumber){
	std::ostringstream msg;
	msg << "GnuplotInterpreter::ImportInterpreter::importCastemFunction : "
	    << "the function '" << function << "' declares "
	    << nb << " variables, not " << varNumber << " as requested";
	throw(std::runtime_error(msg.str()));
      }
      auto func = elm.getCastemFunction(library,function);
      interpreter.addFunction(function,std::make_shared<tfel::math::parser::ExternalCastemFunction>(func,varNumber),
			      b,false);
    } // end of GnuplotInterpreter::ImportInterpreter::importCastemFunction

    void
    GnuplotInterpreter::ImportInterpreter::importCFunction(const std::string& f,
							   const std::string& l,
							   const unsigned short varNumber,
							   const bool b)
    {
      using namespace tfel::math::parser;
      using EFunctionPtr = std::shared_ptr<ExternalFunction>;
      auto& elm = tfel::system::ExternalLibraryManager::getExternalLibraryManager();
      auto declare = [this,&f,b](const EFunctionPtr& ptr){
	interpreter.addFunction(f,ptr,b,false);
      };
      switch (varNumber){
      case 0:
	declare(std::make_shared<ExternalCFunction<0>>(elm.getCFunction0(l,f)));
	break;
      case 1:
	declare(std::make_shared<ExternalCFunction<1>>(elm.getCFunction1(l,f)));
	break;
      case 2:
	declare(std::make_shared<ExternalCFunction<2>>(elm.getCFunction2(l,f)));
	break;
      case 3:
	declare(std::make_shared<ExternalCFunction<3>>(elm.getCFunction3(l,f)));
	break;
      case 4:
	declare(std::make_shared<ExternalCFunction<4>>(elm.getCFunction4(l,f)));
	break;
      case 5:
	declare(std::make_shared<ExternalCFunction<5>>(elm.getCFunction5(l,f)));
	break;
      case 6:
	declare(std::make_shared<ExternalCFunction<6>>(elm.getCFunction6(l,f)));
	break;
      case 7:
	declare(std::make_shared<ExternalCFunction<7>>(elm.getCFunction7(l,f)));
	break;
      case 8:
	declare(std::make_shared<ExternalCFunction<8>>(elm.getCFunction8(l,f)));
	break;
      case 9:
	declare(std::make_shared<ExternalCFunction<9>>(elm.getCFunction9(l,f)));
	break;
      case 10:
	declare(std::make_shared<ExternalCFunction<10>>(elm.getCFunction10(l,f)));
	break;
      case 11:
	declare(std::make_shared<ExternalCFunction<11>>(elm.getCFunction11(l,f)));
	break;
      case 12:
	declare(std::make_shared<ExternalCFunction<12>>(elm.getCFunction12(l,f)));
	break;
      case 13:
	declare(std::make_shared<ExternalCFunction<13>>(elm.getCFunction13(l,f)));
	break;
      case 14:
	declare(std::make_shared<ExternalCFunction<14>>(elm.getCFunction14(l,f)));
	break;
      case 15:
	declare(std::make_shared<ExternalCFunction<15>>(elm.getCFunction15(l,f)));
	break;
      default:
	throw(std::runtime_error("GnuplotInterpreter::ImportInterpreter::importCFunction: "
				 "function with more than 15 variables are not allowed."));
      }
    } // end of GnuplotInterpreter::ImportInterpreter::importCFunction

    void
    GnuplotInterpreter::ImportInterpreter::importFunction(const_iterator& p,
							  const const_iterator pe,
							  const GnuplotInterpreter::ImportInterpreter::ImportOptions options,
							  const bool b)
    {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::ImportInterpreter::importFunction : ",
				      "expected function name",p,pe);
      const auto function = p->value;
      ++p;
      unsigned short varNumber = 0;
      if(options.numberOfVariables==-1){
	CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::ImportInterpreter::importFunction : ",
					"expected variable list",p,pe);
	if(p->value=="("){
	  const auto& vars = this->readVariableList(p,pe);
	  varNumber = static_cast<unsigned short>(vars.size());
	} else {
	  varNumber = 0u;
	}
      } else {
	if(p!=pe){
	  if(p->value=="("){
	    const auto& vars = this->readVariableList(p,pe);
	    if(vars.size()!=static_cast<unsigned short>(options.numberOfVariables)){
	      throw(std::runtime_error("GnuplotInterpreter::ImportInterpreter::importFunction: "
				       "the number variables of function '"+function+"'"
				       "is not the same as that specified in the options"));
	    }
	  }
	}
	varNumber = static_cast<unsigned short>(options.numberOfVariables);
      }
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::ImportInterpreter::importFunction : ",
				      "expected library name",p,pe);
      const auto& library = CxxTokenizer::readString(p,pe);
      // adding the new function
      if((options.functionType==ImportOptions::FDefault)||
	 (options.functionType==ImportOptions::C)){
	this->importCFunction(function,library,varNumber,b);
      } else if (options.functionType==ImportOptions::Castem){
	this->importCastemFunction(function,library,varNumber,b);
      } else {
	throw(std::runtime_error("GnuplotInterpreter::ImportInterpreter::importFunction: "
				 "unknown function type"));
      }
    } // end of GnuplotInterpreter::ImportInterpreter::importFunction()

  } // end of namespace plot

} // end of namespace tfel
