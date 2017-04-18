## Makefile.am -- Process this file with automake to produce a Makefile.in file.

AM_CPPFLAGS = -I$(top_srcdir)/include             \
	      @QT_CXXFLAGS@                       \
	     `@TFELCONFIG@ --includes --cppflags`

LDADD = -L$(top_builddir)/src/Plot   \
	-lTFELPlot                   \
	-lTFELGnuplotInterpreter     \
	-L@TFELHOME@/lib -lTFELTests \
        @QT_LIBS@

testdir = $(pkgdatadir)/tests/
test_PROGRAMS = GnuplotInterpreterTest
GnuplotInterpreterTest_SOURCES = GnuplotInterpreterTest.cxx

TESTS=$(test_PROGRAMS)

EXTRA_DIST=CMakeLists.txt

clean-local:
	-$(RM) GnuplotInterpreterTest.xml
