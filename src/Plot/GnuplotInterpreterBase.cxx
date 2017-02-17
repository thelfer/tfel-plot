/*! 
 * \file  GnuplotInterpreterBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#include<sstream>
#include<iterator>
#include<stdexcept>

#include"TFEL/Math/Evaluator.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"TFEL/Plot/TextDataReader.hxx"
#include"TFEL/Plot/GnuplotInterpreterBase.hxx"

namespace tfel
{

  namespace plot
  {

    GnuplotInterpreterBase::GnuplotInterpreterBase(Graph& graph,
						   QObject *const p)
      : QObject(p),
	g(graph)	
    {} // end of GnuplotInterpreterBase::GnuplotInterpreterBase

    unsigned short
    GnuplotInterpreterBase::convertToUnsignedShort(const std::string& value)
    {
      using namespace std;
      string::const_iterator p;
      istringstream converter(value);
      for(p=value.begin();p!=value.end();++p){
	if(!isdigit(*p)){
	  throw(runtime_error("GnuplotInterpreterBase::convertToUnsignedShort : invalid entry"));
	}
      }
      unsigned short u;
      converter >> u;
      if(!converter&&(!converter.eof())){
	string msg("GnuplotInterpreterBase::convertToUnsignedShort : ");
	msg += "not read value from token '"+value+"'.\n";
	throw(runtime_error(msg));
      }
      return u;
    } // end of GnuplotInterpreterBase::convertToUnsignedShort

    void
    GnuplotInterpreterBase::readVariableList(QVector<std::string>& vars,
					     const_iterator& p,
					     const const_iterator pe,
					     const bool b)
    {
      using namespace std;
      vars.clear();
      if(b){
	CxxTokenizer::readSpecifiedToken("GnuplotInterpreterBase::readVariableList","(",p,pe);
      }
      if(p==pe){
	string msg("GnuplotInterpreterBase::readVariableList : ");
	msg += "unexpected end of line";
	throw(runtime_error(msg));
      }
      if(!GnuplotInterpreterBase::isValidIdentifier(p->value)){
	string msg("GnuplotInterpreterBase::readVariableList : ");
	msg += p->value+" is not a valid identifer.";
	throw(runtime_error(msg));
      }
      vars.push_back(p->value);
      ++p;
      if(b){
	if(p==pe){
	  string msg("GnuplotInterpreterBase::readVariableList : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
      }
      while(((p!=pe))&&(p->value==",")){
	++p;
	CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::readVariableList",
					"expected variable name",p,pe);
	if(p==pe){
	  string msg("GnuplotInterpreterBase::readVariableList : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	if(!GnuplotInterpreterBase::isValidIdentifier(p->value)){
	  string msg("GnuplotInterpreterBase::readVariableList : ");
	  msg += p->value+" is not a valid variable name.";
	  throw(runtime_error(msg));
	}
	vars.push_back(p->value);
	++p;
	if(b){
	  if(p==pe){
	    string msg("GnuplotInterpreterBase::readVariableList : ");
	    msg += "unexpected end of line";
	    throw(runtime_error(msg));
	  }
	}
      } // p!=pe
      if(b){
	CxxTokenizer::readSpecifiedToken("GnuplotInterpreterBase::readVariableList",")",p,pe);
      }
    } // end of GnuplotInterpreterBase::readVariableList

    QVector<std::string>
    GnuplotInterpreterBase::readVariableList(const_iterator& p,
					     const const_iterator pe,
					     const bool b)
    {
      using namespace std;
      QVector<string> vars;
      GnuplotInterpreterBase::readVariableList(vars,p,pe,b);
      return vars;
    } // end of GnuplotInterpreterBase::readVariableList

    bool
    GnuplotInterpreterBase::isValidIdentifier(const std::string& name)
    {
      using namespace tfel::math;
      if(!Evaluator::isValidIdentifier(name)){
	return false;
      }
      return CxxTokenizer::isValidIdentifier(name);
    } // end of GnuplotInterpreterBase::isValidIdentifier

    QString
    GnuplotInterpreterBase::readQString(const_iterator& p,
					const const_iterator pe)
    {
      return QString::fromStdString(CxxTokenizer::readString(p,pe));
    } // end of GnuplotInterpreterBase::readQString

    void
    GnuplotInterpreterBase::throwKeyAlreadyRegistredCallBack(const std::string& k)
    {
      using namespace std;
      string msg("GnuplotInterpreterBase::registerCallBack : ");
      msg += "key '"+k+"' already declared";
      throw(runtime_error(msg));
    } // end of GnuplotInterpreterBase::throwKeyAlreadyRegistredCallBack

    bool
    GnuplotInterpreterBase::isUnsignedInteger(const std::string& s)
    {
      using namespace std;
      string::const_iterator p;
      for(p=s.begin();p!=s.end();++p){
	if(!isdigit(*p)){
	  return false;
	}
      }
      return true;
    } // end of GnuplotInterpreterBase::isUnsignedInteger
  
    std::string
    GnuplotInterpreterBase::readNextGroup(const_iterator& p,
					  const const_iterator pe)
    {
      using namespace std;
      string res;
      unsigned short openedParenthesis=0;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::readNextGroup","",p,pe);
      while((p!=pe)&&(!((p->value==",")&&(openedParenthesis==0)))){
	if(p->value=="("){
	  ++openedParenthesis;
	}
	if(p->value==")"){
	  if(openedParenthesis==0){
	    string msg("GnuplotInterpreterBase::readNextGroup : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	}
	res += p->value;
	++p;
      }
      if(openedParenthesis!=0){
	string msg("GnuplotInterpreterBase::readNextGroup : ");
	msg += "unclosed parenthesis";
	throw(runtime_error(msg));
      }
      return res;
    } // end of GnuplotInterpreterBase::readNextGroup

    double
    GnuplotInterpreterBase::readDouble(const std::string& s,
				       const unsigned short l)
    {
      using namespace std;
      double res;
      istringstream is(s);
      is >> res;
      if(!is&&(!is.eof())){
	ostringstream msg;
	msg << "GnuplotInterpreterBase::readDouble : ";
	msg << "could not read value from token '"+s+"'.\n";
	msg << "Error at line : " << l;
	throw(runtime_error(msg.str()));
      }
      return res;
    } // end of GnuplotInterpreterBase::readDouble

    void
    GnuplotInterpreterBase::readRange(bool& hasRMinValue,
				      bool& hasRMaxValue,
				      double& rmin,
				      double& rmax,
				      tfel::math::parser::ExternalFunctionManagerPtr& functions,
				      const_iterator& p, 
				      const const_iterator pe)
    {
      using namespace std;
      using namespace tfel::math;
      CxxTokenizer::readSpecifiedToken("GnuplotInterpreterBase::readRange","[",p,pe);
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::readRange","expected ':' or a value",p,pe);
      if(p->value!=":"){
	vector<string> vars;
	Evaluator ev(vars,GnuplotInterpreterBase::readUntil(p,pe,":"),functions);
	ev.removeDependencies();
	if(ev.getNumberOfVariables()!=0u){
	  const vector<string>& ev_vars = ev.getVariablesNames();
	  ostringstream msg;
	  if(ev_vars.size()!=1u){
	    msg << "GnuplotInterpreterBase::readRange : invalid range declaration (unknown variables ";
	    copy(ev_vars.begin(),ev_vars.end(),ostream_iterator<string>(msg," "));
	    msg << ")";
	  } else {
	    msg << "GnuplotInterpreterBase::readRange : invalid range declaration (unknown variable "
		<< ev_vars[0] << ")";
	  }
	  throw(runtime_error(msg.str()));
	}
	rmin = ev.getValue();
	hasRMinValue = true;
      } else {
	hasRMinValue = false;
	++p;
      }
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::readRange","expected ']' or a value",p,pe);
      if(p->value!="]"){
	vector<string> vars;
	Evaluator ev(vars,GnuplotInterpreterBase::readUntil(p,pe,"]"),functions);
	ev.removeDependencies();
	if(ev.getNumberOfVariables()!=0u){
	  const vector<string>& ev_vars = ev.getVariablesNames();
	  ostringstream msg;
	  if(ev_vars.size()!=1u){
	    msg << "GnuplotInterpreterBase::readRange : invalid range declaration (unknown variables ";
	    copy(ev_vars.begin(),ev_vars.end(),ostream_iterator<string>(msg," "));
	    msg << ")";
	  } else {
	    msg << "GnuplotInterpreterBase::readRange : invalid range declaration (unknown variable "
		<< ev_vars[0] << ")";
	  }
	  throw(runtime_error(msg.str()));
	}
	rmax = ev.getValue();
	hasRMaxValue = true;
      } else {
	hasRMaxValue = false;
	++p;
      }
    } // end of GnuplotInterpreterBase::readRange

    std::string
    GnuplotInterpreterBase::readUntil(const_iterator& p,
				      const const_iterator pe,
				      const std::string& delim)
    {
      using namespace std;
      string res;
      unsigned short openedParenthesis=0;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::readUntil","",p,pe);
      while((p!=pe)&&(p->value!=delim)){
	if(p->value=="("){
	  ++openedParenthesis;
	}
	if(p->value==")"){
	  if(openedParenthesis==0){
	    string msg("GnuplotInterpreterBase::readUntil : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	}
	res += p->value;
	++p;
      }
      if(p->value!=delim){
	string msg("GnuplotInterpreterBase::readUntil : ");
	msg += "did not find token '"+delim+"'";
	throw(runtime_error(msg));
      }
      if(openedParenthesis!=0){
	string msg("GnuplotInterpreterBase::readUntil : ");
	msg += "unclosed parenthesis";
	throw(runtime_error(msg));
      }
      ++p;
      return res;
    } // end of GnuplotInterpreterBase::readUntil

    void
    GnuplotInterpreterBase::readDataFunctionInUsingDeclaration(std::string& d,
							       const_iterator& p,
							       const const_iterator pe)
    {
      using namespace std;
      unsigned short openedParenthesis;
      CxxTokenizer::checkNotEndOfLine("readDataFunctionInUsingDeclaration",
    				      "expected using declaration",p,pe);
      if(p->value=="("){
    	++p;
    	CxxTokenizer::checkNotEndOfLine("readDataFunctionInUsingDeclaration","",p,pe);
    	openedParenthesis = 0;
    	while(!((p->value==")")&&(openedParenthesis==0))){
    	  if(p->value=="("){
    	    ++openedParenthesis;
    	  }
    	  if(p->value==")"){
    	    if(openedParenthesis==0){
    	      string msg("readDataFunctionInUsingDeclaration : ");
    	      msg += "unbalanced parenthesis";
    	      throw(runtime_error(msg));
    	    }
    	    --openedParenthesis;
    	  }
    	  d += p->value;
    	  ++p;
    	  CxxTokenizer::checkNotEndOfLine("readDataFunctionInUsingDeclaration","",p,pe);
    	}
    	++p;
      } else {
    	// this shall be a column number
    	if(!GnuplotInterpreterBase::isUnsignedInteger(p->value)){
    	  string msg("readDataFunctionInUsingDeclaration : ");
    	  msg += "unexpected token '"+p->value+"', expected column number";
    	  throw(runtime_error(msg));
    	}
    	d = p->value;
    	++p;
      }
    } // end of GnuplotInterpreterBase::readDataFunctionInUsingDeclaration

    void
    GnuplotInterpreterBase::getData(QVector<double>& v,
				    tfel::math::parser::ExternalFunctionManagerPtr functions,
				    const TextDataReader& data,
				    const_iterator& p,
				    const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::getData : ",
    				      "expected column number",p,pe);
      if(p->value=="("){
    	GnuplotInterpreterBase::getData(v,functions,data,
					GnuplotInterpreterBase::readNextGroup(p,pe));
      } else {
    	data.getColumn(v,GnuplotInterpreterBase::convertToUnsignedShort(p->value));
    	++p;
      }
    } // end of GnuplotInterpreterBase::getData

    QString
    GnuplotInterpreterBase::getData(QVector<double>& v,
				    tfel::math::parser::ExternalFunctionManagerPtr functions,
				    const TextDataReader& fdata,
				    const std::string& s)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      using namespace tfel::utilities;
      if(GnuplotInterpreterBase::isUnsignedInteger(s)){
    	unsigned short c = GnuplotInterpreterBase::convertToUnsignedShort(s);
    	fdata.getColumn(v,c);
    	return fdata.getLegend(c);
      } else {
    	// assuming a function
    	vector<pair<string,unsigned short> > vars;
    	Evaluator e(s,functions);
    	const vector<string>& vnames = e.getVariablesNames();
    	if(vnames.empty()){
    	  string msg("GnuplotInterpreterBase::getData : ");
    	  msg += "function '"+s+"' does not declare any variable";
    	  throw(runtime_error(msg));
    	}
    	vector<string>::const_iterator p;
    	QVector<TextDataReader::Line>::const_iterator p2;
    	vector<pair<string,unsigned short> >::const_iterator p3;
    	for(p=vnames.begin();p!=vnames.end();++p){
    	  if(((*p)[0]!='$')){
    	    parser::ExternalFunctionManager::const_iterator p4;
    	    p4 = functions->find(*p);
    	    if(p4==functions->end()){
    	      string msg("GnuplotInterpreterBase::getData : ");
    	      msg += "invalid variable '"+*p+"'";
    	      throw(runtime_error(msg));
    	    }
    	    if(p4->second->getNumberOfVariables()!=0){
    	      string msg("GnuplotInterpreterBase::getData : ");
    	      msg += "invalid variable '"+*p+"'";
    	      throw(runtime_error(msg));
    	    }
    	    e.setVariableValue(*p,p4->second->getValue());
    	  } else {
    	    if(!GnuplotInterpreterBase::isUnsignedInteger(p->substr(1))){
    	      string msg("GnuplotInterpreterBase::getData : ");
    	      msg += "invalid variable name '"+*p;
    	      msg += "' in function '"+s+"'";
    	      throw(runtime_error(msg));
    	    }
    	    const unsigned short vc = GnuplotInterpreterBase::convertToUnsignedShort(p->substr(1));
    	    if(vc==0){
    	      string msg("GnuplotInterpreterBase::getData : ");
    	      msg += "invalid variable name "+*p;
    	      msg += " in function '"+s+"'.";
    	      throw(runtime_error(msg));
    	    }
    	    vars.push_back(make_pair(*p,vc));
    	  }
    	}
    	for(p2=fdata.begin();p2!=fdata.end();++p2){
    	  for(p3=vars.begin();p3!=vars.end();++p3){
    	    if(p2->values.size()<p3->second){
    	      ostringstream msg;
    	      msg << "TextDataReader::getColumn : line '" 
    		  << p2->nbr << "' "
    		  << "does not have '" << p3->second << "' columns.";
    	      throw(runtime_error(msg.str()));
    	    }
    	    e.setVariableValue(p3->first,
    			       p2->values[p3->second-1]);
    	  }
    	  v.push_back(e.getValue());
    	}
      }
      return "";
    } // end of GnuplotInterpreterBase::getData

  } // end of namespace plot

} // end of namespace tfel
