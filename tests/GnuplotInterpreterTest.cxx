/*!
 * \file   GnuplotInterpreterTest.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17 févr. 2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<cmath>
#include<limits>
#include<cstdlib>
#include<cassert>
#include<iostream>

#include<QtCore/QtDebug>
#ifdef TFEL_QT4
#include<QtGui/QApplication>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QApplication>
#endif /* TFEL_QT5 */

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/GnuplotInterpreter.hxx"

struct GnuplotInterpreterTest final
  : public tfel::tests::TestCase
{
   GnuplotInterpreterTest()
    : tfel::tests::TestCase("TFEL/Plot",
			    "GnuplotInterpreterTest")
  {} // end of GnuplotInterpreterTest
  virtual tfel::tests::TestResult
  execute() override
  {
    this->check_nodeps();
    this->check_const();
    this->check_lock();
    return this->result;
  } // end of execute
private:
  void check_nodeps(void){
    const auto eps = 10*std::numeric_limits<double>::epsilon();
    tfel::plot::Graph g;
    tfel::plot::GnuplotInterpreter i(g);
    QString e;
    TFEL_TESTS_ASSERT(i.parseString(e,"nodeps a=10;"));
    TFEL_TESTS_ASSERT(e.isEmpty());
    // defining a const function 
    TFEL_TESTS_ASSERT(!i.parseString(e,"nodeps f(x)=g(x);"));
    TFEL_TESTS_ASSERT(i.parseString(e,"g(x)=cos(x);"));
    TFEL_TESTS_ASSERT(i.parseString(e,"nodeps f(x)=g(x);"));
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
    TFEL_TESTS_ASSERT(i.parseString(e,"g(x)=sin(x);"));
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
  }
  void check_const(void){
    const auto eps = 10*std::numeric_limits<double>::epsilon();
    tfel::plot::Graph g;
    tfel::plot::GnuplotInterpreter i(g);
    QString e;
    TFEL_TESTS_ASSERT(i.parseString(e,"const a=10;"));
    TFEL_TESTS_ASSERT(e.isEmpty());
    TFEL_TESTS_ASSERT(!i.parseString(e,"a=12;"));
    // defining a const function 
    TFEL_TESTS_ASSERT(!i.parseString(e,"const f(x)=g(x);"));
    TFEL_TESTS_ASSERT(i.parseString(e,"g(x)=cos(x);"));
    TFEL_TESTS_ASSERT(i.parseString(e,"const f(x)=g(x);"));
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
    TFEL_TESTS_ASSERT(i.parseString(e,"g(x)=sin(x);"));
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
  }
  void check_lock(void){
    const auto eps = 10*std::numeric_limits<double>::epsilon();
    tfel::plot::Graph g;
    tfel::plot::GnuplotInterpreter i(g);
    QString e;
    TFEL_TESTS_ASSERT(i.parseString(e,"lock a=10;"));
    TFEL_TESTS_ASSERT(e.isEmpty());
    TFEL_TESTS_ASSERT(!i.parseString(e,"a=12;"));
    TFEL_TESTS_ASSERT(i.parseString(e,"lock f(x)=g(x);"));
    TFEL_TESTS_ASSERT(i.parseString(e,"g(x)=cos(x);"));
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
    TFEL_TESTS_ASSERT(i.parseString(e,"g(x)=sin(x);"));
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::sin(1.4))<eps);
    TFEL_TESTS_ASSERT(!i.parseString(e,"f(x)=exp(12*x);"));
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::sin(1.4))<eps);
    TFEL_TESTS_ASSERT(!i.parseString(e,"a=12;"));
  }
};

TFEL_TESTS_GENERATE_PROXY(GnuplotInterpreterTest,
			  "GnuplotInterpreterTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main(int argc,char** argv)
{
  QApplication a(argc,argv);
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("GnuplotInterpreterTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
