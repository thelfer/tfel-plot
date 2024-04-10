/*!
 * \file  GnuplotInterpreterBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#include <sstream>
#include <iterator>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Plot/TextDataReader.hxx"
#include "TFEL/Plot/GnuplotInterpreterBase.hxx"

namespace tfel {

  namespace plot {

    GnuplotInterpreterBase::GnuplotInterpreterBase(Graph& graph,
                                                   QObject* const p)
        : QObject(p),
          g(graph) {}  // end of GnuplotInterpreterBase::GnuplotInterpreterBase

    unsigned short GnuplotInterpreterBase::convertToUnsignedShort(
        const std::string& value) {
      std::istringstream converter(value);
      for (const auto c : value) {
        tfel::raise_if(!isdigit(c),
                       "GnuplotInterpreterBase::convertToUnsignedShort:"
                       " invalid entry");
      }
      unsigned short u;
      converter >> u;
      tfel::raise_if(!converter && (!converter.eof()),
                     "GnuplotInterpreterBase::convertToUnsignedShort: "
                     "not read value from token '" +
                         value + "'.");
      return u;
    }  // end of GnuplotInterpreterBase::convertToUnsignedShort

    void GnuplotInterpreterBase::readVariableList(
        std::vector<std::string>& vars,
        const_iterator& p,
        const const_iterator pe,
        const bool b) {
      const std::string m = "GnuplotInterpreterBase::readVariableList";
      auto throw_if = [&m](const bool c, const std::string& msg) {
        tfel::raise_if(c, m + ": " + msg);
      };
      vars.clear();
      if (b) {
        CxxTokenizer::readSpecifiedToken(m, "(", p, pe);
      }
      tfel::raise_if(p == pe, "unexpected end of line");
      const auto v = tfel::unicode::getMangledString(p->value);
      tfel::raise_if(!GnuplotInterpreterBase::isValidIdentifier(v),
                     p->value + " is not a valid identifer.");
      vars.push_back(p->value);
      ++p;
      tfel::raise_if(b && (p == pe), "unexpected end of line");
      while (((p != pe)) && (p->value == ",")) {
        ++p;
        CxxTokenizer::checkNotEndOfLine(m, "expected variable name", p, pe);
        const auto vn = tfel::unicode::getMangledString(p->value);
        tfel::raise_if(!GnuplotInterpreterBase::isValidIdentifier(vn),
                       "'" + vn + "' is not a valid variable name.");
        vars.push_back(vn);
        ++p;
        throw_if(b && (p == pe), "unexpected end of line");
      }  // p!=pe
      if (b) {
        CxxTokenizer::readSpecifiedToken(m, ")", p, pe);
      }
    }  // end of GnuplotInterpreterBase::readVariableList

    std::vector<std::string> GnuplotInterpreterBase::readVariableList(
        const_iterator& p, const const_iterator pe, const bool b) {
      std::vector<std::string> vars;
      GnuplotInterpreterBase::readVariableList(vars, p, pe, b);
      return vars;
    }  // end of GnuplotInterpreterBase::readVariableList

    bool GnuplotInterpreterBase::isValidIdentifier(const std::string& name) {
      using namespace tfel::math;
      if (!Evaluator::isValidIdentifier(name)) {
        return false;
      }
      return CxxTokenizer::isValidIdentifier(name);
    }  // end of GnuplotInterpreterBase::isValidIdentifier

    QString GnuplotInterpreterBase::readQString(const_iterator& p,
                                                const const_iterator pe) {
      return QString::fromStdString(CxxTokenizer::readString(p, pe));
    }  // end of GnuplotInterpreterBase::readQString

    void GnuplotInterpreterBase::throwKeyAlreadyRegistredCallBack(
        const std::string& k) {
      tfel::raise(
          "GnuplotInterpreterBase::throwKeyAlreadyRegistredCallBack: "
          "key '" +
          k + "' already declared");
    }  // end of
       // GnuplotInterpreterBase::throwKeyAlreadyRegistredCallBack

    bool GnuplotInterpreterBase::isUnsignedInteger(const std::string& s) {
      for (const auto c : s) {
        if (!isdigit(c)) {
          return false;
        }
      }
      return true;
    }  // end of GnuplotInterpreterBase::isUnsignedInteger

    std::string GnuplotInterpreterBase::readNextGroup(const_iterator& p,
                                                      const const_iterator pe) {
      std::string res;
      unsigned short openedParenthesis = 0;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::readNextGroup",
                                      "", p, pe);
      while ((p != pe) && (!((p->value == ",") && (openedParenthesis == 0)))) {
        if (p->value == "(") {
          ++openedParenthesis;
        }
        if (p->value == ")") {
          tfel::raise_if(openedParenthesis == 0,
                         "GnuplotInterpreterBase::readNextGroup: "
                         "unbalanced parenthesis");
          --openedParenthesis;
        }
        res += p->value;
        ++p;
      }
      tfel::raise_if(openedParenthesis != 0,
                     "GnuplotInterpreterBase::readNextGroup: "
                     "unclosed parenthesis");
      return res;
    }  // end of GnuplotInterpreterBase::readNextGroup

    double GnuplotInterpreterBase::readDouble(const std::string& s,
                                              const unsigned short l) {
      double res;
      std::istringstream is(s);
      is >> res;
      tfel::raise_if(!is && (!is.eof()),
                     "GnuplotInterpreterBase::readDouble: "
                     "could not read value from token '" +
                         s +
                         "'.\n"
                         "Error at line: " +
                         std::to_string(l));
      return res;
    }  // end of GnuplotInterpreterBase::readDouble

    void GnuplotInterpreterBase::readRange(
        bool& hasRMinValue,
        bool& hasRMaxValue,
        double& rmin,
        double& rmax,
        tfel::math::parser::ExternalFunctionManagerPtr& functions,
        const_iterator& p,
        const const_iterator pe) {
      using namespace std;
      using namespace tfel::math;
      CxxTokenizer::readSpecifiedToken("GnuplotInterpreterBase::readRange", "[",
                                       p, pe);
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::readRange",
                                      "expected ':' or a value", p, pe);
      if (p->value != ":") {
        vector<string> vars;
        Evaluator ev(vars, GnuplotInterpreterBase::readUntil(p, pe, ":"),
                     functions);
        ev.removeDependencies();
        if (ev.getNumberOfVariables() != 0u) {
          const auto& ev_vars = ev.getVariablesNames();
          ostringstream msg;
          if (ev_vars.size() != 1u) {
            msg << "GnuplotInterpreterBase::readRange : invalid range "
                   "declaration"
                << " (unknown variables ";
            copy(ev_vars.begin(), ev_vars.end(),
                 ostream_iterator<string>(msg, " "));
            msg << ")";
          } else {
            msg << "GnuplotInterpreterBase::readRange : invalid range "
                   "declaration (unknown variable "
                << ev_vars[0] << ")";
          }
          tfel::raise(msg.str());
        }
        rmin = ev.getValue();
        hasRMinValue = true;
      } else {
        hasRMinValue = false;
        ++p;
      }
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::readRange",
                                      "expected ']' or a value", p, pe);
      if (p->value != "]") {
        vector<string> vars;
        Evaluator ev(vars, GnuplotInterpreterBase::readUntil(p, pe, "]"),
                     functions);
        ev.removeDependencies();
        if (ev.getNumberOfVariables() != 0u) {
          const auto& ev_vars = ev.getVariablesNames();
          ostringstream msg;
          if (ev_vars.size() != 1u) {
            msg << "GnuplotInterpreterBase::readRange : invalid range "
                   "declaration (unknown variables ";
            copy(ev_vars.begin(), ev_vars.end(),
                 ostream_iterator<string>(msg, " "));
            msg << ")";
          } else {
            msg << "GnuplotInterpreterBase::readRange : invalid range "
                   "declaration (unknown variable "
                << ev_vars[0] << ")";
          }
          tfel::raise(msg.str());
        }
        rmax = ev.getValue();
        hasRMaxValue = true;
      } else {
        hasRMaxValue = false;
        ++p;
      }
    }  // end of GnuplotInterpreterBase::readRange

    std::string GnuplotInterpreterBase::readUntil(const_iterator& p,
                                                  const const_iterator pe,
                                                  const std::string& delim) {
      std::string res;
      unsigned short openedParenthesis = 0;
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::readUntil", "",
                                      p, pe);
      while ((p != pe) && (p->value != delim)) {
        if (p->value == "(") {
          ++openedParenthesis;
        }
        if (p->value == ")") {
          tfel::raise_if(openedParenthesis == 0,
                         "GnuplotInterpreterBase::readUntil: "
                         "unbalanced parenthesis");
          --openedParenthesis;
        }
        res += p->value;
        ++p;
      }
      tfel::raise_if(p->value != delim,
                     "GnuplotInterpreterBase::readUntil: "
                     "did not find token '" +
                         delim + "'");
      tfel::raise_if(openedParenthesis != 0,
                     "GnuplotInterpreterBase::readUntil: "
                     "unclosed parenthesis");
      ++p;
      return res;
    }  // end of GnuplotInterpreterBase::readUntil

    void GnuplotInterpreterBase::readDataFunctionInUsingDeclaration(
        std::string& d, const_iterator& p, const const_iterator pe) {
      const std::string m(
          "GnuplotInterpreterBase::readDataFunctionInUsingDeclaration:"
          " ");
      unsigned short openedParenthesis;
      CxxTokenizer::checkNotEndOfLine(m, "expected using declaration", p, pe);
      if (p->value == "(") {
        ++p;
        CxxTokenizer::checkNotEndOfLine(m, "", p, pe);
        openedParenthesis = 0;
        while (!((p->value == ")") && (openedParenthesis == 0))) {
          if (p->value == "(") {
            ++openedParenthesis;
          }
          if (p->value == ")") {
            tfel::raise_if(openedParenthesis == 0,
                           m + ": unbalanced parenthesis");
            --openedParenthesis;
          }
          d += p->value;
          ++p;
          CxxTokenizer::checkNotEndOfLine(m, "", p, pe);
        }
        ++p;
      } else {
        // this shall be a column number
        tfel::raise_if(!GnuplotInterpreterBase::isUnsignedInteger(p->value),
                       m + ": unexpected token '" + p->value +
                           "', expected column number");
        d = p->value;
        ++p;
      }
    }  // end of
       // GnuplotInterpreterBase::readDataFunctionInUsingDeclaration

    void GnuplotInterpreterBase::getData(
        std::vector<double>& v,
        tfel::math::parser::ExternalFunctionManagerPtr functions,
        const TextDataReader& data,
        const_iterator& p,
        const const_iterator pe) {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreterBase::getData : ",
                                      "expected column number", p, pe);
      if (p->value == "(") {
        GnuplotInterpreterBase::getData(
            v, functions, data, GnuplotInterpreterBase::readNextGroup(p, pe));
      } else {
        data.getColumn(
            v, GnuplotInterpreterBase::convertToUnsignedShort(p->value));
        ++p;
      }
    }  // end of GnuplotInterpreterBase::getData

    QString GnuplotInterpreterBase::getData(
        std::vector<double>& v,
        tfel::math::parser::ExternalFunctionManagerPtr functions,
        const TextDataReader& fdata,
        const std::string& s) {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      using namespace tfel::utilities;
      auto throw_if = [](const bool c, const std::string& msg) {
        tfel::raise_if(c, "GnuplotInterpreterBase::getData: " + msg);
      };
      if (GnuplotInterpreterBase::isUnsignedInteger(s)) {
        const auto c = GnuplotInterpreterBase::convertToUnsignedShort(s);
        fdata.getColumn(v, c);
        return fdata.getLegend(c);
      } else {
        // assuming a function
        vector<pair<string, unsigned short>> vars;
        Evaluator e(s, functions);
        const auto& vnames = e.getVariablesNames();
        throw_if(vnames.empty(),
                 "function '" + s + "' does not declare any variable");
        vector<string>::const_iterator p;
        std::vector<TextDataReader::Line>::const_iterator p2;
        vector<pair<string, unsigned short>>::const_iterator p3;
        for (p = vnames.begin(); p != vnames.end(); ++p) {
          if (((*p)[0] != '$')) {
            const auto p4 = functions->find(*p);
            throw_if(p4 == functions->end(), "invalid variable '" + *p + "'");
            throw_if(p4->second->getNumberOfVariables() != 0,
                     "invalid variable '" + *p + "'");
            e.setVariableValue(*p, p4->second->getValue());
          } else {
            throw_if(
                !GnuplotInterpreterBase::isUnsignedInteger(p->substr(1)),
                "invalid variable name '" + *p + "' in function '" + s + "'");
            const auto vc =
                GnuplotInterpreterBase::convertToUnsignedShort(p->substr(1));
            throw_if(vc == 0, "invalid variable name '" + *p +
                                  "' in function '" + s + "'.");
            vars.push_back(make_pair(*p, vc));
          }
        }
        for (p2 = fdata.begin(); p2 != fdata.end(); ++p2) {
          for (p3 = vars.begin(); p3 != vars.end(); ++p3) {
            if (p2->values.size() < p3->second) {
              std::ostringstream msg;
              msg << "TextDataReader::getColumn : line '" << p2->nbr << "' "
                  << "does not have '" << p3->second << "' columns.";
              tfel::raise(msg.str());
            }
            e.setVariableValue(p3->first, p2->values[p3->second - 1]);
          }
          v.push_back(e.getValue());
        }
      }
      return "";
    }  // end of GnuplotInterpreterBase::getData

  }  // end of namespace plot

}  // end of namespace tfel
