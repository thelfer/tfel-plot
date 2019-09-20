/*!
 * \file  GnuplotInterpreter.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 juin 2012
 */

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#include <QtCore/QtDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QApplication>

#include "TFEL/Raise.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Plot/Config/GetInstallPath.hxx"
#include "TFEL/Plot/Graph.hxx"
#include "TFEL/Plot/GraphCoordinates.hxx"
#include "TFEL/Plot/GnuplotImportInterpreter.hxx"
#include "TFEL/Plot/GnuplotPlotInterpreter.hxx"
#include "TFEL/Plot/GnuplotSetInterpreter.hxx"
#include "TFEL/Plot/GnuplotUnSetInterpreter.hxx"
#include "TFEL/Plot/GnuplotKrigingInterpreter.hxx"
#include "TFEL/Plot/GnuplotFitInterpreter.hxx"
#include "TFEL/Plot/GnuplotInterpreter.hxx"

namespace tfel {

  namespace plot {

    /*!
     * \return the path to the documentation file if available.
     * If not, an empty string is returned
     * \param[in] k  : keyword
     */
    static std::string getDocumentationFilePath(const std::string& k) {
      const auto root = getInstallPath();
      auto fn = root + "/share/doc/tfel-plot/tplot/gp/" + k + ".md";
      std::ifstream desc{fn};
      if (desc) {
        return fn;
      }
      return "";
    }

    GnuplotInterpreter::ParsingResult::ParsingResult() = default;
    GnuplotInterpreter::ParsingResult::ParsingResult(ParsingResult&&) =
        default;
    GnuplotInterpreter::ParsingResult::ParsingResult(
        const ParsingResult&) = default;
    GnuplotInterpreter::ParsingResult&
    GnuplotInterpreter::ParsingResult::operator=(ParsingResult&&) =
        default;
    GnuplotInterpreter::ParsingResult&
    GnuplotInterpreter::ParsingResult::operator=(const ParsingResult&) =
        default;

    GnuplotInterpreter::GnuplotInterpreter(Graph& graph,
                                           QObject* const p)
        : GnuplotInterpreterBase(graph, p),
          dummyVariable("x"),
          terminal("x11"),
          functions(new tfel::math::parser::ExternalFunctionManager()) {
      this->registerCallBacks();
    }  // end of GnuplotInterpreter::GnuplotInterpreter

    void GnuplotInterpreter::setDummyVariable(const std::string& v) {
      const auto n = tfel::unicode::getMangledString(v);
      tfel::raise_if(!GnuplotInterpreterBase::isValidIdentifier(n),
                     "GnuplotInterpreter::setDummyVariable: "
                     "'" +
                         n + "' is not a valid identifer.");
      this->dummyVariable = n;
    }  // end of GnuplotInterpreter::setDummyVariable

    const std::string& GnuplotInterpreter::getDummyVariable() const {
      return this->dummyVariable;
    }

    void GnuplotInterpreter::registerCallBacks() {
      this->registerCallBack(this->callBacks, "import",
                             &GnuplotInterpreter::treatImport);
      this->registerCallBack(this->callBacks, "const",
                             &GnuplotInterpreter::treatConst);
      this->registerCallBack(this->callBacks, "lock",
                             &GnuplotInterpreter::treatLock);
      this->registerCallBack(this->callBacks, "nodeps",
                             &GnuplotInterpreter::treatNoDeps);
      this->registerCallBack(this->callBacks, "set",
                             &GnuplotInterpreter::treatSet);
      this->registerCallBack(this->callBacks, "unset",
                             &GnuplotInterpreter::treatUnSet);
      this->registerCallBack(this->callBacks, "print",
                             &GnuplotInterpreter::treatPrint);
      this->registerCallBack(this->callBacks, "plot",
                             &GnuplotInterpreter::treatPlot);
      this->registerCallBack(this->callBacks, "include",
                             &GnuplotInterpreter::treatInclude);
      this->registerCallBack(this->callBacks, "quit",
                             &GnuplotInterpreter::treatQuit);
      this->registerCallBack(this->callBacks, "reset",
                             &GnuplotInterpreter::treatReset);
      this->registerCallBack(this->callBacks, "replot",
                             &GnuplotInterpreter::treatRePlot);
      this->registerCallBack(this->callBacks, "kriging",
                             &GnuplotInterpreter::treatKriging);
      this->registerCallBack(this->callBacks, "help",
                             &GnuplotInterpreter::treatHelp);
      this->registerCallBack(this->callBacks, "fit",
                             &GnuplotInterpreter::treatFit);
    }  // end of GnuplotInterpreter::registerCallBacks()

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::parseFile(
        const QString& f) {
      auto append = [](QString& s, const QString& s2) {
        if (s2.isEmpty()) {
          return;
        }
        if (!s.isEmpty()) {
          s += '\n';
        }
        s += s2;
      };
      ParsingResult r;
      QFile file(f);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        r.error = "can't open file '" + f + "'";
        return r;
      }
      QTextStream in(&file);
      QString line;
      unsigned long pos = 0;
      while (!in.atEnd()) {
        line += in.readLine().trimmed();
        ++pos;
        // one shall look for '\' at the end of line and handle line
        // concatenation...
        if (line.isEmpty()) {
          continue;
        }
        if (line.startsWith('#')) {
          line.clear();
          continue;
        }
        if (!line.endsWith('\\')) {
          const auto r2 = this->parseString(line, false);
          if (r2.status == ParsingResult::QUIT) {
            r.status = ParsingResult::QUIT;
          } else if (r2.status == ParsingResult::FAILURE) {
            if (r.status != ParsingResult::QUIT) {
              r.status = ParsingResult::FAILURE;
            }
          }
          append(r.output, r2.output);
          if (!r2.error.isEmpty()) {
            QString msg("GnuplotInterpreter::parseFile: ");
            msg += QObject::tr(
                       "error at line '%1' while parsing file '%2'\n")
                       .arg(pos)
                       .arg(f);
            msg += r2.error;
            append(r.error, msg);
          }
          line.clear();
        } else {
          line.chop(1);
          line.append(' ');
        }
      }
      if (!line.isEmpty()) {
        QString msg("GnuplotInterpreter::parseFile : ");
        msg +=
            QObject::tr("error at line '%1' while parsing file '%2'\n")
                .arg(pos)
                .arg(f);
        msg += QObject::tr("last line ended with a continuation mark");
        append(r.error, msg);
      }
      if (!r.output.isEmpty()) {
        emit outputMsg(r.output);
      }
      if (!r.error.isEmpty()) {
        emit errorMsg(r.error);
      }
      return r;
    }  // end of GnuplotInterpreter::parseFile

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::parseString(
        const QString& l, const bool b) {
      using namespace tfel::utilities;
      auto append = [](QString& s, const QString& s2) {
        if (s2.isEmpty()) {
          return;
        }
        if (!s.isEmpty()) {
          s += '\n';
        }
        s += s2;
      };
      const auto s = l.trimmed();
      ParsingResult r;
      if ((s.isEmpty())) {
        return r;
      }
      if (s.startsWith('#')) {
        return r;
      }
      this->currentLine = s;
      try {
        CxxTokenizer tokenizer;
        tokenizer.treatCharAsString(true);
        tokenizer.parseString(s.toStdString());
        auto p = tokenizer.begin();
        auto pe = tokenizer.end();
        std::vector<TokensContainer> i(1u);
        while (p != pe) {
          if (p->value != ";") {
            i.back().push_back(*p);
          } else {
            i.push_back(TokensContainer());
          }
          ++p;
        }
        for (const auto& tokens : i) {
          if (tokens.empty()) {
            continue;
          }
          p = tokens.begin();
          pe = tokens.end();
          while (p != pe) {
            auto pf = this->callBacks.find(p->value);
            if (pf == this->callBacks.end()) {
              this->analyseFunctionDefinition(p, pe, false, false);
            } else {
              ++p;
              const auto r2 = (this->*(pf->second))(p, pe);
              if (r2.status == ParsingResult::QUIT) {
                r.status = ParsingResult::QUIT;
              } else if (r2.status == ParsingResult::FAILURE) {
                if (r.status != ParsingResult::QUIT) {
                  r.status = ParsingResult::FAILURE;
                }
              }
              append(r.output, r2.output);
              append(r.error, r2.error);
            }
            if (p != pe) {
              tfel::raise(
                  "GnuplotInterpreter::eval: "
                  "unexpected token '" +
                  p->value + "'");
            }
          }
        }
      } catch (std::exception& e) {
        if (r.status != ParsingResult::QUIT) {
          r.status = ParsingResult::FAILURE;
        }
        append(r.error, QString::fromStdString(e.what()));
      } catch (...) {
        if (r.status != ParsingResult::QUIT) {
          r.status = ParsingResult::FAILURE;
        }
        append(r.error, "unknown exception");
      }
      if (b) {
        if (!r.output.isEmpty()) {
          emit outputMsg(r.output);
        }
        if (!r.error.isEmpty()) {
          emit errorMsg(r.error);
        }
      }
      return r;
    }

    void GnuplotInterpreter::setTerminal(
        const std::string& t, const std::vector<std::string>&) {
      if (!((t == "pdf") || (t == "table") || (t == "svg") ||
            (t == "bmp") || (t == "png") || (t == "jpg") ||
            (t == "jpeg") || (t == "ppm") || (t == "xbm") ||
            (t == "xpm") || (t == "x11"))) {
        tfel::raise(
            "GnuplotInterpreter::setTerminal: "
            "unsupported terminal '" +
            t + "'");
      }
      this->terminal = t;
    }  // end of GnuplotInterpreter::setTerminal

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatHelp(
        const_iterator& p, const const_iterator pe) {
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::treatHelp",
                                      "", p, pe);
      ParsingResult r;
      const auto k = p->value;
      const auto f = getDocumentationFilePath(k);
      ++p;
      if (f.empty()) {
        r.output +=
            QString::fromStdString("no help available for '" + k + "'");
      } else {
        std::ifstream desc(f);
        std::ostringstream o;
        if (!desc) {
          // note, this shall never append...
          o << "can't access to the description '" << k << "'";
        } else {
          o << desc.rdbuf();
        }
        r.output += QString::fromStdString(o.str());
      }
      return r;
    }  // end of GnuplotInterpreter::treatHelp

    void GnuplotInterpreter::setOutput(const std::string& o) {
      this->output = o;
    }

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatRePlot(
        const_iterator& p, const const_iterator pe) {
      auto throw_if = [](const bool c, const std::string& m) {
        tfel::raise_if(c, "GnuplotInterpreter::treatRePlot: " + m);
      };
      throw_if(this->previousPlot.isEmpty(), "no previous plot");
      throw_if(p != pe, "unexpected token '" + p->value + "'");
      return this->parseString(this->previousPlot);
    }  // end of GnuplotInterpreter::treatRePlot

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatPlot(
        const_iterator& p, const const_iterator pe) {
      auto throw_if = [](const bool c, const std::string& m) {
        tfel::raise_if(c, "GnuplotInterpreter::treatPlot: " + m);
      };
      PlotInterpreter i(*this, this->g);
      this->g.removeCurves();
      i.eval(p, pe);
      if (this->terminal == "x11") {
        this->g.replot();
        this->g.show();
        emit graphicalPlot();
      } else {
        throw_if(this->output.empty(), "no output file specified");
        throw_if(this->terminal.empty(), "no terminal file specified");
        if (this->terminal == "pdf") {
          this->g.replot();
          this->g.exportToPDF(QString::fromStdString(this->output));
        } else if (this->terminal == "svg") {
          this->g.replot();
          this->g.exportToSVG(QString::fromStdString(this->output));
        } else if (this->terminal == "png") {
          this->g.replot();
          this->g.exportToPNG(QString::fromStdString(this->output));
        } else if (this->terminal == "xbm") {
          this->g.replot();
          this->g.exportToXBM(QString::fromStdString(this->output));
        } else if (this->terminal == "xpm") {
          this->g.replot();
          this->g.exportToXPM(QString::fromStdString(this->output));
        } else if (this->terminal == "ppm") {
          this->g.replot();
          this->g.exportToPPM(QString::fromStdString(this->output));
        } else if (this->terminal == "bmp") {
          this->g.replot();
          this->g.exportToBMP(QString::fromStdString(this->output));
        } else if ((this->terminal == "jpg") ||
                   (this->terminal == "jepg")) {
          this->g.replot();
          this->g.exportToJPEG(QString::fromStdString(this->output));
        } else if (this->terminal == "table") {
          this->g.replot();
          this->g.exportToTable(QString::fromStdString(this->output));
        } else {
          throw_if(true,
                   "internal error, unsupported "
                   "terminal '" +
                       this->terminal + "'");
        }
      }
      this->previousPlot = this->currentLine;
      return {};
    }

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatSet(
        const_iterator& p, const const_iterator pe) {
      SetInterpreter i(*this, this->g);
      i.eval(p, pe);
      return {};
    }

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatUnSet(
        const_iterator& p, const const_iterator pe) {
      UnSetInterpreter i(this->g);
      i.eval(p, pe);
      return {};
    }

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatReset(
        const_iterator&, const const_iterator) {
      this->g.reset();
      return {};
    }  // end of GnuplotInterpreter::treatReset

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatInclude(
        const_iterator& p, const const_iterator pe) {
      const auto f = this->readQString(p, pe);
      return this->parseFile(f);
    }  // end of GnuplotInterpreter::treatInclude

    std::shared_ptr<tfel::math::Evaluator>
    GnuplotInterpreter::readFunction(const_iterator& p,
                                     const const_iterator pe,
                                     const std::string& delim) {
      return this->readFunction(p, pe, std::vector<std::string>{delim});
    }

    std::shared_ptr<tfel::math::Evaluator>
    GnuplotInterpreter::readFunction(
        const_iterator& p,
        const const_iterator pe,
        const std::vector<std::string>& delim) {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::utilities;
      using std::vector;
      vector<string> vars;
      string f;
      unsigned short openedParenthesis = 0;
      bool cont = true;
      auto prev = p;
      auto pb = p;
      while ((p != pe) && (cont)) {
        const bool found =
            find(delim.begin(), delim.end(), p->value) != delim.end();
        if ((found) && (openedParenthesis == 0)) {
          cont = false;
        } else if (p->value == "(") {
          ++openedParenthesis;
          f += p->value;
          prev = p;
          ++p;
        } else if (p->value == ")") {
          tfel::raise_if(openedParenthesis == 0,
                         "GnuplotInterpreter::treatFunctionPlot: "
                         "unbalanced parenthesis");
          --openedParenthesis;
          f += p->value;
          prev = p;
          ++p;
        } else {
          if ((openedParenthesis == 0) && (prev != pb)) {
            if ((prev->value == "+") || (prev->value == "-") ||
                (prev->value == "*") || (prev->value == "/") ||
                (prev->value == "**")) {
              f += p->value;
              prev = p;
              ++p;
            } else {
              cont = false;
            }
          } else {
            f += p->value;
            prev = p;
            ++p;
          }
        }
      }
      tfel::raise_if(openedParenthesis != 0,
                     "GnuplotInterpreter::readFunction: "
                     "unmatched parenthesis");
      return std::make_shared<Evaluator>(vars, f, this->functions);
    }  // end of GnuplotInterpreter::readFunction

    void GnuplotInterpreter::readCoordinates(GraphCoordinates& c,
                                             const_iterator& p,
                                             const const_iterator pe,
                                             const std::string& delim) {
      using namespace std;
      this->readCoordinates(c, p, pe, vector<string>(1u, delim));
    }  // end of GnuplotInterpreter::readCoordinates

    void GnuplotInterpreter::readCoordinates(
        GraphCoordinates& c,
        const_iterator& p,
        const const_iterator pe,
        const std::vector<std::string>& delim) {
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::readCoordinates", "", p, pe);
      if (p->value == "first") {
        c = GraphCoordinates(GraphCoordinates::FIRST,
                             this->readFunction(p, pe, delim));
      } else if (p->value == "second") {
        c = GraphCoordinates(GraphCoordinates::SECOND,
                             this->readFunction(p, pe, delim));
      } else if (p->value == "graph") {
        c = GraphCoordinates(GraphCoordinates::GRAPH,
                             this->readFunction(p, pe, delim));
      } else if (p->value == "screen") {
        c = GraphCoordinates(GraphCoordinates::SCREEN,
                             this->readFunction(p, pe, delim));
      } else if (p->value == "character") {
        c = GraphCoordinates(GraphCoordinates::CHARACTER,
                             this->readFunction(p, pe, delim));
      }
      c = GraphCoordinates(GraphCoordinates::FIRST,
                           this->readFunction(p, pe, delim));
    }  // end of GnuplotInterpreter::readCoordinates

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatQuit(
        const_iterator&, const const_iterator) {
      emit quitCommandTreated();
      ParsingResult r;
      r.status = ParsingResult::QUIT;
      return r;
    }  // end of GnuplotInterpreter::treatQuit

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatImport(
        const_iterator& p, const const_iterator pe) {
      ImportInterpreter i(*this, this->g);
      i.treatImport(p, pe, false);
      return {};
    }  // end of GnuplotInterpreter::treatImport

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatKriging(
        const_iterator& p, const const_iterator pe) {
      KrigingInterpreter i(*this, this->g);
      i.eval(p, pe);
      return {};
    }  // end of GnuplotInterpreter::treatKriging

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatFit(
        const_iterator& p, const const_iterator pe) {
      FitInterpreter i(*this, this->g);
      i.eval(p, pe);
      return {};
    }  // end of GnuplotInterpreter::treatFit

    std::string GnuplotInterpreter::gatherTokenGroup(
        const_iterator& p, const const_iterator pe) {
      auto all = std::string{};
      while (p != pe) {
        all += p->value;
        ++p;
      }
      return all;
    }  // end of GnuplotInterpreter::gatherTokenGroup

    void GnuplotInterpreter::addFunction(
        const std::string& name,
        std::shared_ptr<tfel::math::parser::ExternalFunction> pev,
        const bool b1,
        const bool b2) {
      auto throw_if = [](const bool c, const std::string& m) {
        tfel::raise_if(c, "GnuplotInterpreter::addFunction: " + m);
      };
      const auto mname = tfel::unicode::getMangledString(name);
      throw_if(!this->isValidIdentifier(mname),
               "name '" + name + "' is not valid.");
      if (this->locks.find(name) != this->locks.end()) {
        throw_if((*(this->functions)).find(name) ==
                     (*(this->functions)).end(),
                 "internal error, a lock has been posed for a variable "
                 "called '"
                 "'" +
                     name + "' that apparently don't exist");
        throw_if(true, "a variable '" + name +
                           "' has already been declared, "
                           "with const or lock attribute");
      }
      if (b2) {
        (*(this->functions))[name] = pev->resolveDependencies();
      } else {
        (*(this->functions))[name] = pev;
      }
      if (b1) {
        this->locks.insert(name);
      }
    }  // end of GnuplotInterpreter::addFunction

    void GnuplotInterpreter::analyseFunctionDefinition(
        const_iterator& p,
        const const_iterator pe,
        const bool b1,
        const bool b2) {
      using namespace std;
      using namespace tfel::utilities;
      using tfel::math::Evaluator;
      using tfel::math::parser::ExternalFunction;
      using std::vector;
      auto throw_if = [](const bool c, const std::string& m) {
        if (c) {
          throw(std::runtime_error(
              "GnuplotInterpreter::analyseFunctionDefinition(: " + m));
        }
      };
      throw_if(p == pe, "unexpected end of line");
      const auto variable = p->value;
      const auto v = tfel::unicode::getMangledString(variable);
      // variable or function definition
      throw_if(!this->isValidIdentifier(v),
               "'" + p->value + "' is not a valid identifer");
      ++p;
      throw_if(p == pe, "unexpected end of line");
      if (p->value == "=") {
        vector<string> vars;
        // adding a variable
        ++p;
        throw_if(p == pe, "unexpected end of line");
        const auto group = this->gatherTokenGroup(p, pe);
        throw_if(group.empty(),
                 "invalid declaraction of variable '" + variable + "'");
        auto ev = std::make_shared<Evaluator>(vars, group, functions);
        if (ev->getNumberOfVariables() != 0u) {
          string msg(
              "GnuplotInterpreter::analyseFunctionDefinition : ");
          msg += "error while declaring variable '" + variable;
          if (ev->getNumberOfVariables() == 1u) {
            msg += "', unknown variable " + ev->getVariablesNames()[0];
          } else {
            const auto& evars = ev->getVariablesNames();
            std::vector<string>::const_iterator pv;
            msg += ", unknown variables ";
            for (pv = evars.begin(); pv != evars.end();) {
              msg += *pv;
              ++pv;
              if (pv != evars.end()) {
                msg += ",";
              }
            }
          }
          throw(runtime_error(msg));
        }
        this->addFunction(variable, ev, b1, b2);
      } else if (p->value == "(") {
        // adding a new function
        const auto vars = this->readVariableList(p, pe);
        throw_if(vars.empty(), "no variable defined");
        throw_if(p == pe, "unexpected end of line");
        throw_if(p->value != "=",
                 "unexpected token '" + p->value + "' (expected '=')");
        ++p;
        throw_if(p == pe, "unexpected end of line");
        string group = this->gatherTokenGroup(p, pe);
        throw_if(group.empty(),
                 "invalid declaraction of function '" + variable + "'");
        auto ev = std::make_shared<Evaluator>(vars, group, functions);
        this->addFunction(variable, ev, b1, b2);
      } else {
        throw_if(true, "unexpected token ('" + p->value + "')");
      }
    }  // end of GnuplotInterpreter::analyseFunctionDefinition

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatConst(
        const_iterator& p, const const_iterator pe) {
      this->analyseFunctionDefinition(p, pe, true, true);
      return {};
    }  // end of GnuplotInterpreter::treatConst

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatLock(
        const_iterator& p, const const_iterator pe) {
      this->analyseFunctionDefinition(p, pe, true, false);
      return {};
    }  // end of GnuplotInterpreter::treatLock

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatNoDeps(
        const_iterator& p, const const_iterator pe) {
      this->analyseFunctionDefinition(p, pe, false, true);
      return {};
    }  // end of GnuplotInterpreter::treatNoDeps

    double GnuplotInterpreter::eval(const QString& l) {
      using namespace tfel::utilities;
      auto throw_if = [](const bool c, const std::string& m) {
        if (c) {
          throw(std::runtime_error("GnuplotInterpreter::eval: " + m));
        }
      };
      const auto s = l.trimmed();
      throw_if(s.isEmpty(), "empty string");
      CxxTokenizer tokenizer;
      tokenizer.treatCharAsString(true);
      tokenizer.parseString(s.toStdString());
      auto p = tokenizer.begin();
      return this->eval(p, tokenizer.end());
    }

    double GnuplotInterpreter::eval(const_iterator& p,
                                    const const_iterator pe) {
      using namespace tfel::math;
      using namespace tfel::math::parser;
      auto vars = std::vector<std::string>{};
      const auto gr = this->readNextGroup(p, pe);
      if (gr.empty()) {
        throw(
            std::runtime_error("GnuplotInterpreter::eval: "
                               "invalid expression"));
      }
      auto ev = std::make_shared<Evaluator>(vars, gr, this->functions);
      ev->removeDependencies();
      if (ev->getNumberOfVariables() != 0u) {
        const auto& ev_vars = ev->getVariablesNames();
        std::ostringstream msg;
        if (ev_vars.size() != 1u) {
          msg << "GnuplotInterpreter::eval : invalid declaration "
                 "(unknown variables ";
          std::copy(ev_vars.begin(), ev_vars.end(),
                    std::ostream_iterator<std::string>(msg, " "));
          msg << ")";
        } else {
          msg << "GnuplotInterpreter::eval : invalid declaration "
                 "(unknown variable "
              << ev_vars[0] << ")";
        }
        throw(std::runtime_error(msg.str()));
      }
      return ev->getValue();
    }  // end of GnuplotInterpreter::eval

    GnuplotInterpreter::ParsingResult GnuplotInterpreter::treatPrint(
        const_iterator& p, const const_iterator pe) {
      using namespace tfel::utilities;
      bool cont = true;
      std::ostringstream res;
      res.precision(15);
      CxxTokenizer::checkNotEndOfLine("GnuplotInterpreter::treatPrint",
                                      "", p, pe);
      while ((p != pe) && (cont)) {
        if (p->flag == Token::String) {
          res << p->value.substr(1, p->value.size() - 2);
          ++p;
        } else {
          res << this->eval(p, pe);
        }
        if (p != pe) {
          CxxTokenizer::readSpecifiedToken(
              "GnuplotInterpreter::treatPrint", ",", p, pe);
          CxxTokenizer::checkNotEndOfLine(
              "GnuplotInterpreter::treatPrint", "", p, pe);
        }
      }
      ParsingResult r;
      r.output = QString::fromStdString(res.str());
      return r;
    }  // end of GnuplotInterpreter::treatPrint

    tfel::math::parser::ExternalFunctionManagerPtr
    GnuplotInterpreter::getExternalFunctionManager() {
      return this->functions;
    }

  }  // end of namespace plot

}  // end of namespace tfel
