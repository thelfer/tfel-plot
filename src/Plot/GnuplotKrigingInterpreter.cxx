/*!
 * \file  GnuplotKrigingInterpreter.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 juin 2012
 */

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "TFEL/Math/Parser/KrigedFunction.hxx"
#include "TFEL/Math/Parser/ExternalFunctionManager.hxx"

#include "TFEL/Plot/TextDataReader.hxx"
#include "TFEL/Plot/GnuplotKrigingInterpreter.hxx"

namespace tfel {

  namespace plot {

    GnuplotInterpreter::KrigingInterpreter::KrigingInterpreter(
        GnuplotInterpreter& i, Graph& graph)
        : GnuplotInterpreterBase(graph), interpreter(i) {}

    void GnuplotInterpreter::KrigingInterpreter::eval(
        const_iterator& p, const const_iterator pe) {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      using EFunctionPtr = shared_ptr<ExternalFunction>;
      std::vector<double> vx1;
      std::vector<double> vx2;
      std::vector<double> vx3;
      std::vector<double> vx4;
      std::vector<double> vy;
      int numberOfVariables = -1;
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::treatKriging : ",
          "expected function name", p, pe);
      const auto function = p->value;
      string file;
      ++p;
      unsigned short varNumber = 0;
      auto functions = this->interpreter.getExternalFunctionManager();
      if (numberOfVariables == -1) {
        CxxTokenizer::checkNotEndOfLine(
            "GnuplotInterpreter::treatKriging : ",
            "expected variable list", p, pe);
        if (p->value == "(") {
          const std::vector<string>& vars = this->readVariableList(p, pe);
          varNumber = static_cast<unsigned short>(vars.size());
        } else {
          varNumber = 0u;
        }
      } else {
        if (p != pe) {
          if (p->value == "(") {
            const auto vars = this->readVariableList(p, pe);
            if (vars.size() !=
                static_cast<unsigned short>(numberOfVariables)) {
              string msg("GnuplotInterpreter::treatKriging : ");
              msg +=
                  "the number variables of function '" + function + "'";
              msg += "is not the same as that specified in the options";
              throw(runtime_error(msg));
            }
          }
        }
        varNumber = static_cast<unsigned short>(numberOfVariables);
      }
      if (varNumber == 0) {
        string msg("GnuplotInterpreter::treatKriging : ");
        msg += "the number variables is 0";
        throw(runtime_error(msg));
      }
      CxxTokenizer::checkNotEndOfLine(
          "GnuplotInterpreter::treatKriging : ", "expected file name",
          p, pe);
      TextDataReader data;
      data.extractData(
          QString::fromStdString(CxxTokenizer::readString(p, pe)));
      if (p == pe) {
        if (varNumber == 1) {
          data.getColumn(vx1, 1);
          data.getColumn(vy, 2);
          if (vx1.size() != vy.size()) {
            string msg("GnuplotInterpreter::treatKriging : ");
            msg += "columns don't have the same size";
            throw(runtime_error(msg));
          }
        } else if (varNumber == 2) {
          data.getColumn(vx1, 1);
          data.getColumn(vx2, 2);
          data.getColumn(vy, 3);
        } else if (varNumber == 3) {
          data.getColumn(vx1, 1);
          data.getColumn(vx2, 2);
          data.getColumn(vx3, 3);
          data.getColumn(vy, 4);
        } else {
          string msg("GnuplotInterpreter::treatKriging : ");
          msg += "unsupported number of variables";
          throw(runtime_error(msg));
        }
      } else {
        // using
        CxxTokenizer::readSpecifiedToken(
            "GnuplotInterpreter::treatKriging : ", "using", p, pe);
        CxxTokenizer::checkNotEndOfLine(
            "GnuplotInterpreter::treatKriging", "", p, pe);
        if (varNumber == 1) {
          this->getData(vx1, functions, data, p, pe);
          CxxTokenizer::readSpecifiedToken(
              "GnuplotInterpreter::treatKriging : ", ":", p, pe);
          this->getData(vy, functions, data, p, pe);
        } else if (varNumber == 2) {
          this->getData(vx1, functions, data, p, pe);
          CxxTokenizer::readSpecifiedToken(
              "GnuplotInterpreter::treatKriging : ", ":", p, pe);
          this->getData(vx2, functions, data, p, pe);
          CxxTokenizer::readSpecifiedToken(
              "GnuplotInterpreter::treatKriging : ", ":", p, pe);
          this->getData(vy, functions, data, p, pe);
        } else if (varNumber == 3) {
          this->getData(vx1, functions, data, p, pe);
          CxxTokenizer::readSpecifiedToken(
              "GnuplotInterpreter::treatKriging : ", ":", p, pe);
          this->getData(vx2, functions, data, p, pe);
          CxxTokenizer::readSpecifiedToken(
              "GnuplotInterpreter::treatKriging : ", ":", p, pe);
          this->getData(vx3, functions, data, p, pe);
          CxxTokenizer::readSpecifiedToken(
              "GnuplotInterpreter::treatKriging : ", ":", p, pe);
          this->getData(vy, functions, data, p, pe);
        } else {
          string msg("GnuplotInterpreter::treatKriging : ");
          msg += "unsupported number of variables";
          throw(runtime_error(msg));
        }
      }
      if (varNumber == 1) {
        std::vector<std::pair<double, double>> points(vx1.size());
        decltype(points.size()) i;
        auto pt  = vx1.begin();
        auto pt2 = vy.begin();
        for (i = 0; pt != vx1.end(); ++pt, ++pt2, ++i) {
          points[i] = pair<double, double>(*pt, *pt2);
        }
        interpreter.addFunction(
            function, EFunctionPtr(new KrigedFunction<1>(points)),
            false, false);
      } else if (varNumber == 2) {
        if ((vx1.size() != vx2.size()) || (vx1.size() != vy.size())) {
          string msg("GnuplotInterpreter::treatKriging : ");
          msg += "columns don't have the same size";
          throw(runtime_error(msg));
        }
        std::vector<pair<tvector<2>, double>> points(vx1.size());
        decltype(points.size()) i;
        auto pt  = vx1.begin();
        auto pt2 = vx2.begin();
        auto pt3 = vy.begin();
        for (i = 0; pt != vx1.end(); ++pt, ++pt2, ++pt3, ++i) {
          points[i] = {{*pt,*pt2},*pt3};
        }
        interpreter.addFunction(
            function, EFunctionPtr(new KrigedFunction<2>(points)),
            false, false);
      } else if (varNumber == 3) {
        if ((vx1.size() != vx2.size()) || (vx1.size() != vx3.size()) ||
            (vx1.size() != vy.size())) {
          string msg("GnuplotInterpreter::treatKriging : ");
          msg += "columns don't have the same size";
          throw(runtime_error(msg));
        }
        std::vector<pair<tvector<3>, double>> points(vx1.size());
        decltype(points.size()) i;
        auto pt = vx1.begin();
        auto pt2 = vx2.begin();
        auto pt3 = vx3.begin();
        auto pt4 = vy.begin();
        for (i = 0; pt != vx1.end(); ++pt, ++pt2, ++pt3, ++pt4, ++i) {
          tvector<3> v = {*pt,*pt2,*pt3};
          points[i] = pair<tvector<3>, double>(v, *pt4);
        }
        interpreter.addFunction(
            function, EFunctionPtr(new KrigedFunction<3>(points)),
            false, false);
      } else {
        string msg("GnuplotInterpreter::treatKriging : ");
        msg += "unsupported number of variables";
        throw(runtime_error(msg));
      }
    }  // end of GnuplotInterpreter::KrigingInterpreter::eval

  }  // end of namespace plot

}  // end of namespace tfel
