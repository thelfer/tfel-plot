## Makefile.am -- Process this file with automake to produce a Makefile.in file.

if TFEL_QT4
AM_CPPFLAGS = -I$(top_srcdir)/include             \
	      @QT4_CFLAGS@                        \
	     `@TFELCONFIG@ --includes --cppflags`
endif

if TFEL_QT5
AM_CPPFLAGS = -I$(top_srcdir)/include             \
	      @QT_CXXFLAGS@                       \
	     `@TFELCONFIG@ --includes --cppflags`
endif

LDADD = -L$(top_builddir)/src/Plot -lTFELGnuplotInterpreter \
	-L@TFELHOME@/lib -lTFELTests 


testdir = $(pkgdatadir)/tests/
test_PROGRAMS = GnuplotInterpreterTest
GnuplotInterpreterTest_SOURCES = GnuplotInterpreterTest.cxx

TESTS=$(test_PROGRAMS)

EXTRA_DIST=CMakeLists.txt

clean-local:
	-$(RM) GnuplotInterpreterTest.xml
