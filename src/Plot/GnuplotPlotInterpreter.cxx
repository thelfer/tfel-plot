/*!
 * \file  GnuplotPlotInterpreter.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#include <sstream>
#include <algorithm>

#include <QtCore/QtDebug>

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "TFEL/Plot/Curve.hxx"
#include "TFEL/Plot/Graph.hxx"
#include "TFEL/Plot/GraphTheme.hxx"
#include "TFEL/Plot/DataCurve.hxx"
#include "TFEL/Plot/EvaluatedCurve.hxx"
#include "TFEL/Plot/TextDataReader.hxx"
#include "TFEL/Plot/GnuplotPlotInterpreter.hxx"

namespace tfel {

  namespace plot {

    static qreal convertFromHexadecimal(const char c1, const char c2) {
      static const char hexc[] = {'0', '1', '2', '3', '4', '5',
                                  '6', '7', '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f'};
      const auto i1 = static_cast<unsigned short>(
          std::find(hexc, hexc + 16, tolower(c1)) - hexc);
      const auto i2 = static_cast<unsigned short>(
          std::find(hexc, hexc + 16, tolower(c2)) - hexc);
      if ((i1 > 15) || (i2 > 15)) {
        throw(std::runtime_error(
            "convertFromHexadecimal : invalid input"));
      }
      return static_cast<qreal>((i1 << 4) + i2);
    }  // end of convertFromHexadecimal

    struct GnuplotInterpreter::PlotInterpreter::CurveOptions {
      void setColor(const QColor& c) { this->color = c; }
      //! curve title (can be empty)
      std::string title;
      std::string using_decl_x;
      std::string using_decl_y;
      //! curve style
      Curve::Style style = Curve::SOLIDLINE;
      //! graph axis
      Graph::GraphAxis axis = Graph::XY;
      //! curve color
      QColor color;
      //! curve width
      unsigned short width = 0u;
      //! true if the curve color has been defined
      bool hasSpecifiedColor = false;
      //! true if the curve width has been defined
      bool hasSpecifiedWidth = false;
      //! true if the curve title has been defined
      bool hasTitle = false;
    };  // end of struct
        // GnuplotInterpreter::PlotInterpreter::CurveOptions

    GnuplotInterpreter::PlotInterpreter::PlotInterpreter(
        GnuplotInterpreter& i, Graph& graph)
        : GnuplotInterpreterBase(graph), interpreter(i) {
      this->registerCallBacks();
    }  // end of GnuplotInterpreter::PlotInterpreter::PlotInterpreter

    void GnuplotInterpreter::PlotInterpreter::registerCallBacks() {
      this->registerCallBack(this->plotCallBacks, "t",
                             &PlotInterpreter::treatTitle);
      this->registerCallBack(this->plotCallBacks, "title",
                             &PlotInterpreter::treatTitle);
      this->registerCallBack(this->plotCallBacks, "w",
                             &PlotInterpreter::treatWith);
      this->registerCallBack(this->plotCallBacks, "with",
                             &PlotInterpreter::treatWith);
      this->registerCallBack(this->plotCallBacks, "using",
                             &PlotInterpreter::treatUsing);
      this->registerCallBack(this->plotCallBacks, "u",
                             &PlotInterpreter::treatUsing);
      this->registerCallBack(this->plotCallBacks, "axes",
                             &PlotInterpreter::treatAxes);
      this->registerCallBack(this->plotCallBacks, "axis",
                             &PlotInterpreter::treatAxes);
      this->registerCallBack(this->plotCallBacks, "lt",
                             &PlotInterpreter::treatLineType);
      this->registerCallBack(this->plotCallBacks, "linetype",
                             &PlotInterpreter::treatLineType);
      this->registerCallBack(this->plotCallBacks, "lc",
                             &PlotInterpreter::treatLineColor);
      this->registerCallBack(this->plotCallBacks, "linecolor",
                             &PlotInterpreter::treatLineColor);
      this->registerCallBack(this->plotCallBacks, "pointsize",
                             &PlotInterpreter::treatLineWidth);
      this->registerCallBack(this->plotCallBacks, "ps",
                             &PlotInterpreter::treatLineWidth);
      this->registerCallBack(this->plotCallBacks, "lw",
                             &PlotInterpreter::treatLineWidth);
      this->registerCallBack(this->plotCallBacks, "linewidth",
                             &PlotInterpreter::treatLineWidth);
      this->registerCallBack(this->plotCallBacks, "notitle",
                             &PlotInterpreter::treatNoTitle);
    }  // end of GnuplotInterpreter::PlotInterpreter::registerCallBack

    void GnuplotInterpreter::PlotInterpreter::eval(
        const_iterator& p, const const_iterator pe) {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      bool xrange = false;
      bool yrange = false;
      bool hasXMinValue = false;
      bool hasXMaxValue = false;
      bool hasYMinValue = false;
      bool hasYMaxValue = false;
      qreal xmin;
      qreal xmax;
      qreal ymin;
      qreal ymax;
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::eval", p, pe);
      if (p->value == "[") {
        xrange = true;
        auto efcts = this->interpreter.getExternalFunctionManager();
        GnuplotInterpreterBase::readRange(hasXMinValue, hasXMaxValue,
                                          xmin, xmax, efcts, p, pe);
      }
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::eval", "", p, pe);
      if (p->value == "[") {
        yrange = true;
        auto efcts = this->interpreter.getExternalFunctionManager();
        GnuplotInterpreterBase::readRange(hasYMinValue, hasYMaxValue,
                                          ymin, ymax, efcts, p, pe);
      }
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::eval", "", p, pe);
      if ((xrange) && (!hasXMinValue) && (!hasXMaxValue)) {
        g.unsetXRange();
      }
      if ((yrange) && (!hasYMinValue) && (!hasYMaxValue)) {
        g.unsetYRange();
      }
      if ((hasXMinValue) && (hasXMaxValue)) {
        this->g.setXRange(xmin, xmax);
      }
      if ((hasYMinValue) && (hasYMaxValue)) {
        this->g.setYRange(ymin, ymax);
      }
      while (p != pe) {
        if (p->flag == Token::String) {
          this->treatDataPlot(p, pe);
        } else {
          this->treatFunctionPlot(p, pe);
        }
        if (p != pe) {
          if (p->value != ",") {
            string msg("GnuplotInterpreter::PlotInterpreter::eval : ");
            msg += "unexpected token '" + p->value + "'";
            throw(runtime_error(msg));
          }
          ++p;
          CxxTokenizer::checkNotEndOfLine(
              "GnuplotInterpreter::PlotInterpreter::eval", "", p, pe);
        }
      }
      if ((hasXMinValue || hasXMaxValue) &&
          (!((hasXMinValue) && (hasXMaxValue)))) {
        qreal x0;
        qreal x1;
        this->g.getRange(x0, x1, Graph::xaxis);
        if (hasXMinValue) {
          x0 = xmin;
        }
        if (hasXMaxValue) {
          x1 = xmax;
        }
        this->g.setXRange(x0, x1);
      }
      if ((hasYMinValue || hasYMaxValue) &&
          (!((hasYMinValue) && (hasYMaxValue)))) {
        qreal y0;
        qreal y1;
        this->g.getRange(y0, y1, Graph::yaxis);
        if (hasYMinValue) {
          y0 = ymin;
        }
        if (hasYMaxValue) {
          y1 = ymax;
        }
        this->g.setYRange(y0, y1);
      }
    }  // end of GnuplotInterpreter::PlotInterpreter::eval

    GnuplotInterpreter::PlotInterpreter::CurveOptions
    GnuplotInterpreter::PlotInterpreter::treatPlotOptions(
        const_iterator& p, const const_iterator pe) {
      using namespace std;
      CurveOptions options;
      while ((p != pe) && (p->value != ",")) {
        auto ps = this->plotCallBacks.find(p->value);
        if (ps == this->plotCallBacks.end()) {
          string msg(
              "GnuplotInterpreter::PlotInterpreter::treatPlotOptions "
              ": ");
          msg += "unknown option '" + p->value + "'";
          throw(runtime_error(msg));
        }
        ++p;
        (this->*(ps->second))(options, p, pe);
      }
      return options;
    }  // end of GnuplotInterpreter::PlotInterpreter::treatPlotOptions

    void GnuplotInterpreter::PlotInterpreter::applyCurveOptions(
        std::shared_ptr<Curve> c,
        const GnuplotInterpreter::PlotInterpreter::CurveOptions&
            options) {
      if (options.hasTitle) {
        c->setKey(QString::fromStdString(options.title));
      }
      if (options.hasSpecifiedColor) {
        c->setColor(options.color);
      }
      if (options.hasSpecifiedWidth) {
        c->setWidth(options.width);
      }
      c->setStyle(options.style);
    }  // end of GnuplotInterpreter::PlotInterpreter::applyCurveOptions

    void GnuplotInterpreter::PlotInterpreter::treatFunctionPlot(
        const_iterator& p, const const_iterator pe) {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      auto throw_if = [](const bool c, const std::string& m) {
        if (c) {
          throw(
              std::runtime_error("GnuplotInterpreter::PlotInterpreter::"
                                 "treatFunctionPlot: " +
                                 m));
        }
      };
      vector<string> vars(1, interpreter.getDummyVariable());
      unsigned short openedParenthesis = 0;
      bool cont = true;
      auto prev = p;
      string f;
      while ((p != pe) && (cont)) {
        if (p->value == "(") {
          ++openedParenthesis;
        }
        if (p->value == ")") {
          throw_if(openedParenthesis == 0, "unbalanced parenthesis");
          --openedParenthesis;
        }
        if (p->value == ",") {
          if (openedParenthesis == 0) {
            cont = false;
          } else {
            f += p->value;
            prev = p;
            ++p;
          }
        } else if (this->plotCallBacks.find(p->value) !=
                   this->plotCallBacks.end()) {
          if (openedParenthesis == 0) {
            // this might be the title
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
        } else {
          f += p->value;
          prev = p;
          ++p;
        }
      }
      throw_if(f.empty(), "no function given");
      auto efcts = this->interpreter.getExternalFunctionManager();
      shared_ptr<Evaluator> ev(new Evaluator(vars, f, efcts));
      ev->removeDependencies();
      const auto& options = this->treatPlotOptions(p, pe);
      throw_if((!options.using_decl_x.empty()) ||
                   (!options.using_decl_y.empty()),
               "the 'using' keyword is invalid for functions");
      shared_ptr<Curve> c(new EvaluatedCurve(
          ev, QString::fromStdString(interpreter.getDummyVariable())));
      this->applyCurveOptions(c, options);
      if (!options.hasTitle) {
        c->setKey(QString::fromStdString(f));
      }
      // here treating options
      this->g.addCurve(c, options.axis);
    }  // end of GnuplotInterpreter::PlotInterpreter::treatFunctionPlot

    void GnuplotInterpreter::PlotInterpreter::treatDataPlot(
        const_iterator& p, const const_iterator pe) {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      string file = CxxTokenizer::readString(p, pe);
      string x;
      string y;
      const CurveOptions& options = this->treatPlotOptions(p, pe);
      if (options.using_decl_x.empty()) {
        x = "1";
      } else {
        x = options.using_decl_x;
      }
      if (options.using_decl_y.empty()) {
        y = "2";
      } else {
        y = options.using_decl_y;
      }
      QString qfile = QString::fromStdString(file);
      QString qx = QString::fromStdString(x);
      QString qy = QString::fromStdString(y);
      auto efcts = this->interpreter.getExternalFunctionManager();
      const bool bcx = GnuplotInterpreterBase::isUnsignedInteger(x);
      const bool bcy = GnuplotInterpreterBase::isUnsignedInteger(y);
      unsigned short cx = 1;
      unsigned short cy = 2;
      if (bcx) {
        cx = GnuplotInterpreterBase::convertToUnsignedShort(x);
      }
      if (bcy) {
        cy = GnuplotInterpreterBase::convertToUnsignedShort(y);
      }
      shared_ptr<Curve> c;
      QString ty;
      if ((bcx) && (bcy)) {
        c = shared_ptr<Curve>(new DataCurve(qfile, "", cx, cy));
        TextDataReader d;
        d.extractData(QString::fromStdString(file));
        ty = d.getLegend(cy);
      } else if ((!bcx) && (bcy)) {
        c = shared_ptr<Curve>(new DataCurve(qfile, "", qx, cy, efcts));
        TextDataReader d;
        d.extractData(QString::fromStdString(file));
        ty = d.getLegend(cy);
      } else if ((bcx) && (!bcy)) {
        c = shared_ptr<Curve>(new DataCurve(qfile, "", cx, qy, efcts));
      } else {
        c = shared_ptr<Curve>(new DataCurve(qfile, "", qx, qy, efcts));
      }
      this->applyCurveOptions(c, options);
      if (!options.hasTitle) {
        if (ty.isEmpty()) {
          string legend = '\"' + file + '\"';
          legend += " using ";
          if (GnuplotInterpreterBase::isUnsignedInteger(x)) {
            legend += x;
          } else {
            legend += "(" + x + ")";
          }
          legend += ":";
          if (GnuplotInterpreterBase::isUnsignedInteger(y)) {
            legend += y;
          } else {
            legend += "(" + y + ")";
          }
          c->setKey(QString::fromStdString(legend));
        } else {
          c->setKey(ty);
        }
      }
      // here treating options
      this->g.addCurve(c, options.axis);
    }  // end of GnuplotInterpreter::PlotInterpreter::treatDataPlot

    void GnuplotInterpreter::PlotInterpreter::treatLineType(
        CurveOptions& options,
        const_iterator& p,
        const const_iterator pe) {
      auto throw_if = [](const bool c, const std::string& m) {
        if (c) {
          throw(
              std::runtime_error("GnuplotInterpreter::PlotInterpreter::"
                                 "treatLineType: " +
                                 m));
        }
      };
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::treatType",
          "expected line type", p, pe);
      throw_if(options.hasSpecifiedColor,
               "line type already specified");
      if (p->value == "rgb") {
        ++p;
        CxxTokenizer::checkNotEndOfLine(
            "GnuplotInterpreter::PlotInterpreter::treatType",
            "expected line type", p, pe);
        const auto type = CxxTokenizer::readString(p, pe);
        throw_if(type.empty(), "empty line type specified");
        if (type[0] == '#') {
          throw_if(type.size() != 7, "invalid hexadecimal number");
          options.color =
              QColor(convertFromHexadecimal(type[1], type[2]),
                     convertFromHexadecimal(type[3], type[4]),
                     convertFromHexadecimal(type[5], type[6]));
        } else {
          throw_if(true, "expected token starting with '#'");
        }
      } else {
        if (GnuplotInterpreter::PlotInterpreter::isUnsignedInteger(
                p->value)) {
          this->g.getTheme()->getDefaultColor(
              options.color, this->convertToUnsignedShort(p->value));
        } else {
          options.color =
              Graph::getColorByName(QString::fromStdString(p->value));
        }
        ++p;
      }
      options.hasSpecifiedColor = true;
    }  // end of GnuplotInterpreter::PlotInterpreter::treatLineType

    void GnuplotInterpreter::PlotInterpreter::treatLineColor(
        CurveOptions& options,
        const_iterator& p,
        const const_iterator pe) {
      auto throw_if = [](const bool c, const std::string& m) {
        if (c) {
          throw(
              std::runtime_error("GnuplotInterpreter::PlotInterpreter::"
                                 "treatLineColor: " +
                                 m));
        }
      };
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::treatLineColor",
          "expected line type", p, pe);
      throw_if(options.hasSpecifiedColor,
               "line type already specified");
      if (p->value == "rgb") {
        ++p;
        CxxTokenizer::checkNotEndOfLine(
            "GnuplotInterpreter::PlotInterpreter::treatLineColor",
            "expected line type", p, pe);
        const auto type = CxxTokenizer::readString(p, pe);
        throw_if(type.empty(), "empty line type specified");
        if (type[0] == '#') {
          // rgb
          throw_if(type.size() != 7, "invalid hexadecimal number");
          options.color =
              QColor(convertFromHexadecimal(type[1], type[2]),
                     convertFromHexadecimal(type[3], type[4]),
                     convertFromHexadecimal(type[5], type[6]));
        } else {
          throw_if(true, "unexpected token '#'");
        }
      } else {
        if (GnuplotInterpreter::PlotInterpreter::isUnsignedInteger(
                p->value)) {
          this->g.getTheme()->getDefaultColor(
              options.color, this->convertToUnsignedShort(p->value));
        } else {
          options.color =
              Graph::getColorByName(QString::fromStdString(p->value));
        }
        ++p;
      }
      options.hasSpecifiedColor = true;
    }  // end of GnuplotInterpreter::PlotInterpreter::treatLineColor

    void GnuplotInterpreter::PlotInterpreter::treatLineWidth(
        CurveOptions& options,
        const_iterator& p,
        const const_iterator pe) {
      auto throw_if = [](const bool c, const std::string& m) {
        if (c) {
          throw(
              std::runtime_error("GnuplotInterpreter::PlotInterpreter::"
                                 "treatLineWidth: " +
                                 m));
        }
      };
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::treatWidth",
          "expected line type", p, pe);
      throw_if(options.hasSpecifiedWidth,
               "line width already specified");
      throw_if(!GnuplotInterpreter::PlotInterpreter::isUnsignedInteger(
                   p->value),
               "unexpected a number");
      options.width = this->convertToUnsignedShort(p->value);
      options.hasSpecifiedWidth = true;
      ++p;
    }  // end of GnuplotInterpreter::PlotInterpreter::treatLineWidth

    void GnuplotInterpreter::PlotInterpreter::treatAxes(
        CurveOptions& options,
        const_iterator& p,
        const const_iterator pe) {
      auto throw_if = [](const bool c, const std::string& m) {
        if (c) {
          throw(
              std::runtime_error("GnuplotInterpreter::PlotInterpreter::"
                                 "treatAxes: " +
                                 m));
        }
      };
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::treatAxes",
          "expected axis name", p, pe);
      throw_if(options.axis != Graph::XY, "axes already specified");
      if (p->value == "x1y1") {
        options.axis = Graph::XY;
      } else if (p->value == "x1y2") {
        options.axis = Graph::XY2;
      } else if (p->value == "x2y1") {
        options.axis = Graph::X2Y;
      } else if (p->value == "x2y2") {
        options.axis = Graph::X2Y2;
      } else {
        throw_if(true, "unknown axis '" + p->value + "'");
      }
      ++p;
    }  // end of GnuplotInterpreter::PlotInterpreter::treatAxes

    void GnuplotInterpreter::PlotInterpreter::treatUsing(
        CurveOptions& options,
        const_iterator& p,
        const const_iterator pe) {
      if ((!options.using_decl_x.empty()) ||
          (!options.using_decl_y.empty())) {
        throw(std::runtime_error(
            "GnuplotInterpreter::PlotInterpreter::treatUsing: "
            "using option already specified."));
      }
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::treatUsing",
          "expected using declaration", p, pe);
      GnuplotInterpreterBase::readDataFunctionInUsingDeclaration(
          options.using_decl_x, p, pe);
      CxxTokenizer::readSpecifiedToken(
          "GnuplotInterpreter::PlotInterpreter::treatUsing", ":", p,
          pe);
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::treatUsing", "", p, pe);
      GnuplotInterpreterBase::readDataFunctionInUsingDeclaration(
          options.using_decl_y, p, pe);
    }  // end of GnuplotInterpreter::PlotInterpreter::treatUsing

    void GnuplotInterpreter::PlotInterpreter::treatTitle(
        CurveOptions& options,
        const_iterator& p,
        const const_iterator pe) {
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::treatTitle",
          "expected curve legend", p, pe);
      if (options.hasTitle) {
        throw(std::runtime_error(
            "GnuplotInterpreter::PlotInterpreter::treatTitle: "
            "curve legend already specified"));
      }
      options.hasTitle = true;
      options.title = CxxTokenizer::readString(p, pe);
    }  // end of GnuplotInterpreter::PlotInterpreter::treatTitle

    void GnuplotInterpreter::PlotInterpreter::treatWith(
        CurveOptions& options,
        const_iterator& p,
        const const_iterator pe) {
      auto throw_if = [](const bool c, const std::string& m) {
        if (c) {
          throw(
              std::runtime_error("GnuplotInterpreter::PlotInterpreter::"
                                 "treatWith: " +
                                 m));
        }
      };
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::PlotInterpreter::treatTitle",
          "expected curve legend", p, pe);
      throw_if(options.style != Curve::SOLIDLINE,
               "curve style already specified");
      if ((p->value == "l") || (p->value == "lines")) {
        options.style = Curve::SOLIDLINE;
      } else if (p->value == "linespoints") {
        options.style = Curve::DOTLINE;
      } else if ((p->value == "d") || (p->value == "dots")) {
        options.style = Curve::DOT;
      } else if ((p->value == "p") || (p->value == "points")) {
        options.style = Curve::DOT;
      } else if ((p->value == "t") || (p->value == "triangles")) {
        options.style = Curve::TRIANGLE;
      } else if ((p->value == "c") || (p->value == "crosses")) {
        options.style = Curve::CROSS;
      } else if (p->value == "diamonds") {
        options.style = Curve::DIAMOND;
      } else if ((p->value == "s") || (p->value == "square")) {
        options.style = Curve::SQUARE;
      } else if (p->value == "dashline") {
        options.style = Curve::DASHLINE;
      } else {
        throw_if(true, "unsupported style '" + p->value + "'");
      }
      ++p;
    }  // end of GnuplotInterpreter::PlotInterpreter::treatWith

    void GnuplotInterpreter::PlotInterpreter::treatNoTitle(
        CurveOptions&, const_iterator&, const const_iterator) {
    }  // end of GnuplotInterpreter::PlotInterpreter::treatNoTitle

  }  // end of namespace plot

}  // end of namespace tfel
