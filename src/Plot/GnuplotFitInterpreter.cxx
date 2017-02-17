/*! 
 * \file  GnuplotFitInterpreter.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 juin 2012
 */

#include<sstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"TFEL/Math/Evaluator.hxx"
#include"TFEL/Math/Parser/KrigedFunction.hxx"
#include"TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include"TFEL/Math/LevenbergMarquardt.hxx"
#include"TFEL/Math/LevenbergMarquardt/LevenbergMarquardtFunctionWrapper.hxx"
#include"TFEL/Math/LevenbergMarquardt/LevenbergMarquardtExternalFunctionWrapper.hxx"

#include"TFEL/Plot/TextDataReader.hxx"
#include"TFEL/Plot/GnuplotFitInterpreter.hxx"

namespace tfel
{

  namespace plot
  {

    GnuplotInterpreter::FitInterpreter::FitInterpreter(GnuplotInterpreter& i,
						       Graph& graph)
      : GnuplotInterpreterBase(graph),
	interpreter(i)
    {}

    void
    GnuplotInterpreter::FitInterpreter::eval(const_iterator& p, 
					     const const_iterator pe)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      using namespace tfel::utilities;
      using std::vector;
      typedef LevenbergMarquardtExternalFunctionWrapper ExternalFunctionWrapper;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::treatFit","",p,pe);
      shared_ptr<Evaluator> ev;
      QVector<QVector<double> > values;
      QVector<string> params;
      set<string> ev_params;
      tfel::math::vector<double> v_values;
      tfel::math::vector<double> p_values;
      vector<string> columns;
      QVector<string> vars;
      string f = p->value;
      string function;
      string file;
      QVector<string>::const_iterator ps;
      ExternalFunctionManager::const_iterator pf;
      ExternalFunctionManager::iterator pf2;
      QVector<string>::size_type i;
      vector<string>::size_type j;
      vector<double>::size_type size;
      if(!this->isValidIdentifier(f)){
    	string msg("GnuplotInterpreter::treatFit : '");
    	msg += f+"' is not a valid function name.";
    	throw(runtime_error(msg));
      }
      ++p;
      this->readVariableList(vars,p,pe);
      if(vars.empty()){
    	string msg("GnuplotInterpreter::treatFit : ");
    	msg += "no variable defined for function '"+f+"'";
    	throw(runtime_error(msg));
      }
      function=f+'(';
      for(ps=vars.begin();ps!=vars.end();){
    	function+=*ps;
    	if(++ps!=vars.end()){
    	  function+=',';
    	}
      }
      function+=')';
      shared_ptr<ExternalFunctionManager> functions;
      functions = this->interpreter.getExternalFunctionManager();
      ev = std::make_shared<Evaluator>(vars.toStdVector(),function,functions);
      vars = QVector<string>::fromStdVector(ev->getVariablesNames());
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::treatFit","",p,pe);
      file = CxxTokenizer::readString(p,pe);
      columns.resize(vars.size()+1);
      if((p->value=="using")||
    	 (p->value=="u")){
    	++p;
    	CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::treatFit",
    					"expected using declaration",p,pe);
    	for(i=0;i!=vars.size();++i){
    	  GnuplotInterpreterBase::readDataFunctionInUsingDeclaration(columns[i],p,pe);
    	  CxxTokenizer::readSpecifiedToken("GnuplotInterpreter::treatFit",":",p,pe);
    	}
    	GnuplotInterpreterBase::readDataFunctionInUsingDeclaration(columns[i],p,pe);
      } else {
    	for(i=0;i!=vars.size()+1;++i){
    	  ostringstream converter;
    	  converter << i+1;
    	  columns[i] = converter.str();
    	}
      }
      CxxTokenizer::readSpecifiedToken("GnuplotInterpreter::treatFit","via",p,pe);
      this->readVariableList(params,p,pe,false);
      if(params.empty()){
    	string msg("GnuplotInterpreter::treatFit : ");
    	msg += "no parameter defined for function '"+f+"'";
    	throw(runtime_error(msg));
      }
      for(ps=vars.begin();ps!=vars.end();++ps){
    	if(find(params.begin(),params.end(),*ps)!=params.end()){
    	  string msg("GnuplotInterpreter::treatFit : ");
    	  msg += "'"+*ps+"' is both a variable and a parameter";
    	  throw(runtime_error(msg));
    	}
      }
      // preparing the evaluator
      shared_ptr<ExternalFunction> nev = ev->createFunctionByChangingParametersIntoVariables(params.toStdVector());
      // reading data
      values.resize(vars.size()+1);
      TextDataReader data(QString::fromStdString(file));
      for(i=0;i!=vars.size()+1;++i){
    	GnuplotInterpreterBase::getData(values[i],functions,data,columns[i]);
      }
      size = values[0].size();
      for(i=1;i!=vars.size()+1;++i){
    	if(values[i].size()!=size){
    	  string msg("GnuplotInterpreter::treatFit : ");
    	  msg += "data columns does not have the same size";
    	  throw(runtime_error(msg));
    	}
      }
      v_values.resize(vars.size());
      vector<string> v;
      LevenbergMarquardt<ExternalFunctionWrapper> levmar(ExternalFunctionWrapper(nev,vars.size(),params.size()));
      for(i=0;i!=size;++i){
    	for(j=0;j!=vars.size();++j){
    	  v_values[j] = values[j][i];
    	}
    	levmar.addData(v_values,values[vars.size()][i]);
      }
      // clean-up
      values.clear();
      // initial guess
      p_values.resize(params.size());
      for(i=0,ps=params.begin();ps!=params.end();++ps,++i){
    	pf = functions->find(*ps);
    	if(pf==functions->end()){
    	  string msg("GnuplotInterpreter::treatFit : ");
    	  msg += "no initial value for parameter '"+*ps+"'";
    	  throw(runtime_error(msg));
    	}
    	if(pf->second->getNumberOfVariables()!=0){
    	  string msg("GnuplotInterpreter::treatFit : '");
    	  msg += *ps+"' is not a parameter";
    	  throw(runtime_error(msg));
    	}
    	p_values[i] = pf->second->getValue();
      }
      levmar.setInitialGuess(p_values);
      // execute
      p_values = levmar.execute();
      ostringstream res;
      res.precision(15);
      res << "levenberg-marquart converged in "
      	   << levmar.getNumberOfIterations() << " iterations" << endl;
      res << "Fit results : " << endl;
      for(i=0,ps=params.begin();ps!=params.end();++ps,++i){
      	pf2 = functions->find(*ps);
      	if(pf2==functions->end()){
      	  string msg("GnuplotInterpreter::treatFit : ");
      	  msg += "internal error (no parameter '"+*ps+"')";
      	  throw(runtime_error(msg));
      	}
      	if(pf2->second->getNumberOfVariables()!=0){
      	  string msg("GnuplotInterpreter::treatFit : '");
      	  msg += *ps+"' is not a parameter";
      	  throw(runtime_error(msg));
      	}
      	pf2->second = shared_ptr<ExternalFunction>(new Evaluator(p_values[i]));
      	res << " - " << *ps << " = " << p_values[i] << endl;
      }
      this->interpreter.outputMsg(QString::fromStdString(res.str()));
    } // end of GnuplotInterpreter::FitInterpreter::eval

  } // end of namespace plot

} // end of namespace tfel
