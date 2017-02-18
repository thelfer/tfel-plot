/*!
 * \file   GnuplotInterpreterTest.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17 févr. 2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cmath>

#include<cassert>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

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
    return this->result;
  } // end of execute
private:
};

TFEL_TESTS_GENERATE_PROXY(GnuplotInterpreterTest,
			  "GnuplotInterpreterTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("GnuplotInterpreterTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
