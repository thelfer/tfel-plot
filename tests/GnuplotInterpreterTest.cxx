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
#include<QtWidgets/QApplication>
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
  tfel::tests::TestResult execute() override
  {
    this->check_nodeps();
    // this->check_const();
    // this->check_lock();
    return this->result;
  } // end of execute
private:
  using ParsingResult = tfel::plot::GnuplotInterpreter::ParsingResult;
  QString parse(tfel::plot::GnuplotInterpreter& i,const char* const s,
		const ParsingResult::Status f = ParsingResult::SUCCESS){
    const auto r = i.parseString(s);
    TFEL_TESTS_ASSERT(r.status==f);
    return r.error;
  }
  void check_nodeps(){
    const auto eps = 10*std::numeric_limits<double>::epsilon();
    tfel::plot::Graph g;
    tfel::plot::GnuplotInterpreter i(g);
    TFEL_TESTS_ASSERT(this->parse(i,"nodeps a=10;").isEmpty());
    // defining a const function 
    this->parse(i,"nodeps f(x)=g(x);",ParsingResult::FAILURE);
    this->parse(i,"g(x)=cos(x);");
    this->parse(i,"nodeps f(x)=g(x);");
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
    this->parse(i,"g(x)=sin(x);");
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
  }
  void check_const(){
    const auto eps = 10*std::numeric_limits<double>::epsilon();
    tfel::plot::Graph g;
    tfel::plot::GnuplotInterpreter i(g);
    QString e;
    TFEL_TESTS_ASSERT(this->parse(i,"const a=10;").isEmpty());
    this->parse(i,"a=12;",ParsingResult::FAILURE);
    // defining a const function 
    this->parse(i,"const f(x)=g(x);",ParsingResult::FAILURE);
    this->parse(i,"g(x)=cos(x);");
    this->parse(i,"const f(x)=g(x);");
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
    this->parse(i,"g(x)=sin(x);");
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
  }
  void check_lock(){
    const auto eps = 10*std::numeric_limits<double>::epsilon();
    tfel::plot::Graph g;
    tfel::plot::GnuplotInterpreter i(g);
    TFEL_TESTS_ASSERT(this->parse(i,"lock a=10;").isEmpty());
    this->parse(i,"a=12;",ParsingResult::FAILURE);
    this->parse(i,"lock f(x)=g(x);");
    this->parse(i,"g(x)=cos(x);");
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::cos(1.4))<eps);
    this->parse(i,"g(x)=sin(x);");
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::sin(1.4))<eps);
    this->parse(i,"f(x)=exp(12*x);",ParsingResult::FAILURE);
    TFEL_TESTS_ASSERT(std::abs(i.eval("f(1.4)")-std::sin(1.4))<eps);
    this->parse(i,"a=12;",ParsingResult::FAILURE);
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
