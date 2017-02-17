/*! 
 * \file  GnuplotInterpreter.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 juin 2012
 */

#include<cstdlib>
#include<sstream>
#include<stdexcept>
#include<algorithm>

#include<QtCore/QtDebug>
#include<QtCore/QFile>
#include<QtCore/QTextStream>
#ifdef TFEL_QT4
#include<QtGui/QApplication>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QApplication>
#endif /* TFEL_QT5 */

#include"TFEL/Math/Evaluator.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/GraphCoordinates.hxx"
#include"TFEL/Plot/GnuplotImportInterpreter.hxx"
#include"TFEL/Plot/GnuplotPlotInterpreter.hxx"
#include"TFEL/Plot/GnuplotSetInterpreter.hxx"
#include"TFEL/Plot/GnuplotUnSetInterpreter.hxx"
#include"TFEL/Plot/GnuplotKrigingInterpreter.hxx"
#include"TFEL/Plot/GnuplotFitInterpreter.hxx"
#include"TFEL/Plot/GnuplotInterpreter.hxx"

namespace tfel
{

  namespace plot
  {

    GnuplotInterpreter::GnuplotInterpreter(Graph& graph,
					   QObject *const p)
      : GnuplotInterpreterBase(graph,p),
	dummyVariable("x"),
	terminal("x11"),
	functions(new tfel::math::parser::ExternalFunctionManager())
    {
      this->registerCallBacks();
    } // end of GnuplotInterpreter::GnuplotInterpreter

    void
    GnuplotInterpreter::setDummyVariable(const std::string& n)
    {
      using namespace std;
      if(!GnuplotInterpreterBase::isValidIdentifier(n)){
	string msg("GnuplotInterpreter::setDummyVariable : '");
	msg += n+"' is not a valid identifer.";
	throw(runtime_error(msg));
      }
      this->dummyVariable = n;
    }// end of GnuplotInterpreter::setDummyVariable
    
    const std::string&
    GnuplotInterpreter::getDummyVariable() const
    {
      return this->dummyVariable;
    }

    void
    GnuplotInterpreter::registerCallBacks()
    {
      this->registerCallBack(this->callBacks,"import",
			     &GnuplotInterpreter::treatImport);
      this->registerCallBack(this->callBacks,"const",
			     &GnuplotInterpreter::treatConst);
      this->registerCallBack(this->callBacks,"lock",
			     &GnuplotInterpreter::treatLock);
      this->registerCallBack(this->callBacks,"nodeps",
			     &GnuplotInterpreter::treatNoDeps);
      this->registerCallBack(this->callBacks,"set",
			     &GnuplotInterpreter::treatSet);
      this->registerCallBack(this->callBacks,"unset",
      			     &GnuplotInterpreter::treatUnSet);
      this->registerCallBack(this->callBacks,"print",
			     &GnuplotInterpreter::treatPrint);
      this->registerCallBack(this->callBacks,"plot",
      			     &GnuplotInterpreter::treatPlot);
      this->registerCallBack(this->callBacks,"include",
      			     &GnuplotInterpreter::treatInclude);
      this->registerCallBack(this->callBacks,"quit",
			     &GnuplotInterpreter::treatQuit);
      this->registerCallBack(this->callBacks,"reset",
      			     &GnuplotInterpreter::treatReset);
      this->registerCallBack(this->callBacks,"replot",
			     &GnuplotInterpreter::treatRePlot);
      this->registerCallBack(this->callBacks,"kriging",
			     &GnuplotInterpreter::treatKriging);
      this->registerCallBack(this->callBacks,"fit",
			     &GnuplotInterpreter::treatFit);
    } // end of GnuplotInterpreter::registerCallBacks()

    bool
    GnuplotInterpreter::parseFile(QString& e,
				  const QString& f)
    {
      QFile file(f);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
	return false;
      }
      QTextStream in(&file);
      QString line;
      unsigned long pos = 0;
      while (!in.atEnd()) {
	line += in.readLine().trimmed ();
	++pos;
	// one shall look for '\' at the end of line and handle line
	// concatenation...
	if(line.isEmpty()){
	  continue;
	}
	if(line.startsWith('#')){
	  line.clear();
	  continue;
	}
	if(!line.endsWith('\\')){
	  QString le;
	  if(!this->eval(le,line,false)){
	    QString msg("GnuplotInterpreter::parseFile : ");
	    msg += QObject::tr("error at line '%1' while parsing file '%2'\n").arg(pos).arg(f);
	    msg += le;
	    if(!e.isEmpty()){
	      e += "\n";
	    }
	    e += msg;
	  }
	  line.clear();
	} else {
	  line.chop(1);
	  line.append(' ');
	}
      }
      if(!line.isEmpty()){
	QString msg("GnuplotInterpreter::parseFile : ");
	msg += QObject::tr("error at line '%1' while parsing file '%2'\n").arg(pos).arg(f);
	msg += QObject::tr("last line ended with a continuation mark");
	if(!e.isEmpty()){
	  e += "\n";
	}
	e += msg;
      }
      if(!e.isEmpty()){
	emit errorMsg(e);
	return false;
      }
      return true;
    } // end of GnuplotInterpreter::parseFile
    
    bool
    GnuplotInterpreter::eval(QString& error,
			     const QString& l,
			     const bool b)
    {
      using namespace std;
      using namespace tfel::utilities;
      const QString s = l.trimmed();
      if(s.isEmpty()){
	return true;
      }
      if(s.startsWith('#')){
	return true;
      }
      this->currentLine = s;
      try{
	map<string,MemFuncPtr>::const_iterator pf;
	CxxTokenizer tokenizer;
	tokenizer.treatCharAsString(true);
	CxxTokenizer::const_iterator p;
	CxxTokenizer::const_iterator pe;
	tokenizer.parseString(s.toStdString());
	p=tokenizer.begin();
	pe=tokenizer.end();
	vector<TokensContainer> i(1u);
	while(p!=pe){
	  if(p->value!=";"){
	    i.back().push_back(*p);
	  } else {
	    i.push_back(TokensContainer());
	  }
	  ++p;
	}
	vector<TokensContainer>::const_iterator pi;
	for(pi=i.begin();pi!=i.end();++pi){
	  const TokensContainer& tokens = *pi;
	  p=tokens.begin();
	  pe=tokens.end();
	  if(tokens.empty()){
	    continue;
	  }
	  while(p!=pe){
	    pf = this->callBacks.find(p->value);
	    if(pf==this->callBacks.end()){
	      this->analyseFunctionDefinition(p,pe,false,false);
	    } else {
	      ++p;
	      (this->*(pf->second))(p,pe);
	    }
	    if(p!=pe){
	      string msg("GnuplotInterpreter::eval : ");
	      msg += "unexpected token '"+p->value+"'";
	      throw(runtime_error(msg));
	    }
	  }
	}
      } catch(std::exception& e){
	error = e.what();
	if(b){
	  emit errorMsg(e.what());
	}
	return false;
      } catch(...){
	error = QObject::tr("unknown exception"); 
	if(b){
	  emit errorMsg(error);
	}
	return false;
      }
      return true;
    }

    void
    GnuplotInterpreter::setTerminal(const std::string& t,
				    const std::vector<std::string>&)
    {
      using namespace std;
      if((t=="eps")||(t=="pdf")||	 
	 (t=="table")||(t=="svg")||
	 (t=="bmp")||(t=="png")||
	 (t=="jpg")||(t=="jpeg")||
	 (t=="ppm")||(t=="xbm")||
	 (t=="xpm")||(t=="x11")){
	this->terminal=t;
      } else {
	string msg("Analyser::treatSetTerminal : ");
	msg += "unsupported terminal '"+t+"'";
	throw(runtime_error(msg));
      }
    } // end of GnuplotInterpreter::setTerminal

    void
    GnuplotInterpreter::setOutput(const std::string& o)
    {
      this->output = o;
    }

    void
    GnuplotInterpreter::treatRePlot(const_iterator& p,
				    const const_iterator pe)
    {
      using namespace std;
      if(this->previousPlot.isEmpty()){
	string msg("GnuplotInterpreter::treatPlot : ");
	msg += "no previous plot";
	throw(runtime_error(msg));
      }
      if(p!=pe){
	string msg("GnuplotInterpreter::treatRePlot : ");
	msg += "unexpected token '"+p->value+"'";
	throw(runtime_error(msg));
      }
      QString e;
      this->eval(e,this->previousPlot);
    } // end of 

    void
    GnuplotInterpreter::treatPlot(const_iterator& p,
				  const const_iterator pe)
    {
      using namespace std;
      PlotInterpreter i(*this,this->g);
      this->g.removeCurves();
      i.eval(p,pe);
      if(this->terminal=="x11"){
	this->g.replot();
      	this->g.show();
      } else {
      	if(this->output.empty()){
      	  string msg("GnuplotInterpreter::treatPlot : ");
      	  msg += "no output file specified";
      	  throw(runtime_error(msg));
      	}
      	if(this->terminal.empty()){
      	  string msg("GnuplotInterpreter::treatPlot : ");
      	  msg += "no terminal file specified";
      	  throw(runtime_error(msg));
      	}
	if(this->terminal=="pdf"){
	  this->g.replot();
      	  this->g.exportToPDF(QString::fromStdString(this->output));
#ifdef TFEL_QT4
	} else if(this->terminal=="eps"){
	  this->g.replot();
      	  this->g.exportToPostScript(QString::fromStdString(this->output));
#endif /* TFEL_QT4 */	  
      	} else if(this->terminal=="svg"){
	  this->g.replot();
      	  this->g.exportToSVG(QString::fromStdString(this->output));
      	} else if(this->terminal=="png"){
	  this->g.replot();
      	  this->g.exportToPNG(QString::fromStdString(this->output));
      	} else if(this->terminal=="xbm"){
	  this->g.replot();
      	  this->g.exportToXBM(QString::fromStdString(this->output));
      	} else if(this->terminal=="xpm"){
	  this->g.replot();
	  this->g.exportToXPM(QString::fromStdString(this->output));
      	} else if(this->terminal=="ppm"){
	  this->g.replot();
      	  this->g.exportToPPM(QString::fromStdString(this->output));
      	} else if(this->terminal=="bmp"){
	  this->g.replot();
      	  this->g.exportToBMP(QString::fromStdString(this->output));
      	} else if((this->terminal=="jpg")||
		  (this->terminal=="jepg")){
	  this->g.replot();
      	  this->g.exportToJPEG(QString::fromStdString(this->output));
      	} else if(this->terminal=="table"){
	  this->g.replot();
      	  this->g.exportToTable(QString::fromStdString(this->output));
      	} else {
      	  string msg("GnuplotInterpreter::treatPlot : ");
      	  msg += "internal error, unsupported terminal '"+this->terminal+"'";
      	  throw(runtime_error(msg));
      	}
      }
      this->previousPlot = this->currentLine;
    }

    void
    GnuplotInterpreter::treatSet(const_iterator& p,
				 const const_iterator pe)
    {
      SetInterpreter i(*this,this->g);
      i.eval(p,pe);
    }

    void
    GnuplotInterpreter::treatUnSet(const_iterator& p,
				   const const_iterator pe)
    {
      UnSetInterpreter i(this->g);
      i.eval(p,pe);
    }

    void
    GnuplotInterpreter::treatReset(const_iterator&, 
				   const const_iterator)
    {
      this->g.reset();
    } // end of GnuplotInterpreter::treatReset

    void
    GnuplotInterpreter::treatInclude(const_iterator& p, 
				     const const_iterator pe)
    {
      QString f = this->readQString(p,pe);
      QString e;
      this->parseFile(e,f);
    } // end of GnuplotInterpreter::treatInclude

    std::shared_ptr<tfel::math::Evaluator>
    GnuplotInterpreter::readFunction(const_iterator& p, 
    				     const const_iterator pe,
				     const std::string& delim)
    {
      return this->readFunction(p,pe,{delim});
    }

    std::shared_ptr<tfel::math::Evaluator>
    GnuplotInterpreter::readFunction(const_iterator& p, 
    				     const const_iterator pe,
				     const std::vector<std::string>& delim)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::utilities;
      using std::vector;
      vector<string> vars;
      string f;
      unsigned short openedParenthesis = 0;
      bool cont=true;
      const_iterator prev = p;
      const_iterator pb   = p;
      while((p!=pe)&&(cont)){
	const bool found = find(delim.begin(),delim.end(),p->value)!=delim.end();
    	if((found)&&(openedParenthesis==0)){
    	  cont = false;
    	} else if(p->value=="("){
    	  ++openedParenthesis;
    	  f += p->value;
    	  prev=p;
    	  ++p;
    	} else if(p->value==")"){
    	  if(openedParenthesis==0){
    	    string msg("GnuplotInterpreter::treatFunctionPlot : ");
    	    msg += "unbalanced parenthesis";
    	    throw(runtime_error(msg));
    	  }
    	  --openedParenthesis;
    	  f += p->value;
    	  prev=p;
    	  ++p;
    	} else {
    	  if((openedParenthesis==0)&&(prev!=pb)){
    	    if((prev->value=="+")||
    	       (prev->value=="-")||
    	       (prev->value=="*")||
    	       (prev->value=="/")||
    	       (prev->value=="**")){
    	      f += p->value;
    	      prev=p;
    	      ++p;
    	    } else {
    	      cont = false;
    	    }
    	  } else {
    	    f += p->value;
    	    prev=p;
    	    ++p;
    	  }
    	}
      }
      if(openedParenthesis!=0){
    	throw(runtime_error("GnuplotInterpreter::readFunction: "
			    "unmatched parenthesis"));
      }
      return std::make_shared<Evaluator>(vars,f,this->functions);
    } // end of GnuplotInterpreter::readFunction

    void
    GnuplotInterpreter::readCoordinates(GraphCoordinates& c,
					const_iterator& p, 
					const const_iterator pe,
					const std::string& delim)
    {
      using namespace std;
      this->readCoordinates(c,p,pe,vector<string>(1u,delim));
    } // end of GnuplotInterpreter::readCoordinates

    void
    GnuplotInterpreter::readCoordinates(GraphCoordinates& c,
					const_iterator& p, 
					const const_iterator pe,
					const std::vector<std::string>& delim)
    {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::readCoordinates","",p,pe);
      if(p->value=="first"){
	c = GraphCoordinates(GraphCoordinates::FIRST,this->readFunction(p,pe,delim));
      } else if(p->value=="second"){
	c = GraphCoordinates(GraphCoordinates::SECOND,this->readFunction(p,pe,delim));
      } else if(p->value=="graph"){
	c = GraphCoordinates(GraphCoordinates::GRAPH,this->readFunction(p,pe,delim));
      } else if(p->value=="screen"){
	c = GraphCoordinates(GraphCoordinates::SCREEN,this->readFunction(p,pe,delim));
      } else if(p->value=="character"){
	c = GraphCoordinates(GraphCoordinates::CHARACTER,this->readFunction(p,pe,delim));
      }
      c = GraphCoordinates(GraphCoordinates::FIRST,this->readFunction(p,pe,delim));
    } // end of GnuplotInterpreter::readCoordinates


    void
    GnuplotInterpreter::treatQuit(const_iterator&, 
				  const const_iterator)
    {
      QApplication::exit(EXIT_SUCCESS);
    } // end of GnuplotInterpreter::treatQuit
    
    void
    GnuplotInterpreter::treatImport(const_iterator& p,
				    const const_iterator pe)
    {
      ImportInterpreter i(*this,this->g);
      i.treatImport(p,pe,false);
    } // end of GnuplotInterpreter::treatImport

    void
    GnuplotInterpreter::treatKriging(const_iterator& p,
				    const const_iterator pe)
    {
      KrigingInterpreter i(*this,this->g);
      i.eval(p,pe);
    } // end of GnuplotInterpreter::treatKriging

    void
    GnuplotInterpreter::treatFit(const_iterator& p,
				    const const_iterator pe)
    {
      FitInterpreter i(*this,this->g);
      i.eval(p,pe);
    } // end of GnuplotInterpreter::treatFit

    std::string
    GnuplotInterpreter::gatherTokenGroup(const_iterator& p,
					 const const_iterator pe)
    {
      auto all =  std::string{};
      while(p!=pe){
	all += p->value;
	++p;
      }
      return all;
    } // end of GnuplotInterpreter::gatherTokenGroup

    void
    GnuplotInterpreter::addFunction(const std::string& name,
				    std::shared_ptr<tfel::math::parser::ExternalFunction> pev,
				    const bool b1,
				    const bool b2)
    {
      using namespace std;
      if(!this->isValidIdentifier(name)){
	string msg("GnuplotInterpreter::addFunction : ");
	msg += "name '"+name+"' is not valid.";
	throw(runtime_error(msg));
      }
      if(this->locks.find(name)!=this->locks.end()){
	if((*(this->functions)).find(name)==(*(this->functions)).end()){
	  string msg("GnuplotInterpreter::addFunction : ");
	  msg += "internal error, a lock has been posed for a variable called '";
	  msg += name;
	  msg += "' that apparently don't exist";
	  throw(runtime_error(msg));
	}
	string msg("GnuplotInterpreter::addFunction : ");
	msg += "a variable '"+name+"' has already been declared, with const attribute";
	throw(runtime_error(msg));
      }
      if(b1){
	this->locks.insert(name);
      }
      if(b2){
	(*(this->functions))[name]=pev->resolveDependencies();
      } else {
	(*(this->functions))[name]=pev;
      }
    } // end of GnuplotInterpreter::addFunction

    void
    GnuplotInterpreter::analyseFunctionDefinition(const_iterator& p,
						  const const_iterator pe,
						  const bool b1,
						  const bool b2)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      using std::vector;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::analyseFunctionDefinition","",p,pe);
      string var = p->value;
      // variable or function definition
      if(!this->isValidIdentifier(var)){
	string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	msg += p->value+" is not a valid identifer.";
	throw(runtime_error(msg));
      }
      ++p;
      if(p==pe){
	string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	msg += "unexpected end of line";
	throw(runtime_error(msg));
      }
      if(p->value=="="){
	vector<string> vars;
	// adding a variable
	++p;
	if(p==pe){
	  string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	string group = this->gatherTokenGroup(p,pe);
	if(group.empty()){
	  string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	  msg += "invalid declaraction of variable "+var;
	  throw(runtime_error(msg));
	}
	shared_ptr<tfel::math::parser::ExternalFunction> pev(new Evaluator(vars,group,functions));
	Evaluator* ev = static_cast<Evaluator *>(pev.get());
	if(ev->getNumberOfVariables()!=0u){
	  string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	  msg += "error while declaring variable "+var;
	  if(ev->getNumberOfVariables()==1u){
	    msg += ", unknown variable "+ev->getVariablesNames()[0];
	  } else {
	    const std::vector<string>& evars = ev->getVariablesNames();		
	    std::vector<string>::const_iterator pv;
	    msg += ", unknown variables ";
	    for(pv=evars.begin();pv!=evars.end();){
	      msg+=*pv;
	      ++pv;
	      if(pv!=evars.end()){
		msg+=",";
	      }
	    }
	  }
	  throw(runtime_error(msg));
	}
	this->addFunction(var,pev,b1,b2);
      } else if (p->value=="("){
	shared_ptr<tfel::math::parser::ExternalFunction> ev;
	vector<string>::const_iterator p2;
	// adding a new function
	QVector<string> vars = this->readVariableList(p,pe);
	if(vars.isEmpty()){
	  string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	  msg += "no variable defined";
	  throw(runtime_error(msg));
	}
	if(p==pe){
	  string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	if(p->value!="="){
	  string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	  msg += "unexpected token '"+p->value+"' (expected '=')";
	  throw(runtime_error(msg));
	}
	++p;
	if(p==pe){
	  string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	string group = this->gatherTokenGroup(p,pe);
	if(group.empty()){
	  string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	  msg += "invalid declaraction of function "+var;
	  throw(runtime_error(msg));
	}
	ev = shared_ptr<tfel::math::parser::ExternalFunction> (new Evaluator(vars.toStdVector(),
									     group,functions));
	this->addFunction(var,ev,b1,b2);
      } else {
	string msg("GnuplotInterpreter::analyseFunctionDefinition : ");
	msg += "unexpected token ('"+p->value+"')";
	throw(runtime_error(msg));
      }
    } // end of GnuplotInterpreter::analyseFunctionDefinition

    std::vector<std::string>
    GnuplotInterpreter::tokenize(const std::string& s,
				 const char c)
    {
      using namespace std;
      using std::vector;
      vector<string> res;
      string::size_type b = 0u;
      string::size_type e = s.find_first_of(c, b);
      while (string::npos != e || string::npos != b){
	// Found a token, add it to the vector.
	res.push_back(s.substr(b, e - b));
	b = s.find_first_not_of(c, e);
	e = s.find_first_of(c, b);
      }
      return res;
    } // end of GnuplotInterpreter::tokenize

    void
    GnuplotInterpreter::treatConst(const_iterator& p,
				   const const_iterator pe)
    {
      this->analyseFunctionDefinition(p,pe,true,true);
    } // end of GnuplotInterpreter::treatConst

    void
    GnuplotInterpreter::treatLock(const_iterator& p,
				  const const_iterator pe)
    {
      this->analyseFunctionDefinition(p,pe,true,false);
    } // end of GnuplotInterpreter::treatLock

    void
    GnuplotInterpreter::treatNoDeps(const_iterator& p,
				    const const_iterator pe)
    {
      this->analyseFunctionDefinition(p,pe,false,true);
    } // end of GnuplotInterpreter::treatNoDeps

    void
    GnuplotInterpreter::treatPrint(const_iterator& p,
				   const const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      using std::vector;
      vector<string> vars;
      bool cont = true;
      ostringstream res;
      res.precision(15);
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::treatPrint","",p,pe);
      while((p!=pe)&&(cont)){
	if(p->flag==Token::String){
	  res << p->value.substr(1,p->value.size()-2);
	  ++p;
	} else {
	  string group = this->readNextGroup(p,pe);
	  if(group.empty()){
	    string msg("GnuplotInterpreter::treatPrint : ");
	    msg += "invalid expression";
	    throw(runtime_error(msg));
	  }
	  shared_ptr<Evaluator> ev(new Evaluator(vars,group,this->functions));
	  ev->removeDependencies();
	  if(ev->getNumberOfVariables()!=0u){
	    const vector<string>& ev_vars = ev->getVariablesNames();
	    ostringstream msg;
	    if(ev_vars.size()!=1u){
	      msg << "GnuplotInterpreter::treatPrint : invalid print declaration (unknown variables ";
	      copy(ev_vars.begin(),ev_vars.end(),ostream_iterator<string>(msg," "));
	      msg << ")";
	    } else {
	      msg << "GnuplotInterpreter::treatPrint : invalid print declaration (unknown variable "
		  << ev_vars[0] << ")";
	    }
	    throw(runtime_error(msg.str()));
	  }
	  res << ev->getValue();
	}
	if(p!=pe){
	  CxxTokenizer::readSpecifiedToken("GnuplotInterpreter::treatPrint : ",
					   ",",p,pe);
	  CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::treatPrint : ","",p,pe);
	}
      }
      emit outputMsg(QString::fromStdString(res.str()));
    } // end of GnuplotInterpreter::treatPrint

    tfel::math::parser::ExternalFunctionManagerPtr
    GnuplotInterpreter::getExternalFunctionManager()
    {
      return this->functions;
    }
    
  } // end of namespace plot

} // end of namespace tfel

