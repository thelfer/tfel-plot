## Makefile.am -- Process this file with automake to produce a Makefile.in file.

localedir = $(datadir)/locale

if TFEL_QT4
AM_CPPFLAGS = -I$(top_srcdir)/tplot/include        \
	      -I$(top_srcdir)/include              \
	      @QT4_CFLAGS@                         \
	      `@TFELCONFIG@ --includes --cppflags` \
	      -DLOCALEDIR=\"$(localedir)\"
endif

if TFEL_QT5
AM_CPPFLAGS = -I$(top_srcdir)/tplot/include        \
	      -I$(top_srcdir)/include              \
	      @QT_CXXFLAGS@                        \
	      `@TFELCONFIG@ --includes --cppflags` \
	      -DLOCALEDIR=\"$(localedir)\"
endif

bin_PROGRAMS  = tplot
tplot_SOURCES = tplot.cxx     \
		TPlot.cxx     \
		resources.cxx
nodist_tplot_SOURCES = moc_tplot.cxx 

if TFEL_QT4
moc_tplot.cxx : @top_srcdir@/tplot/include/TFEL/Plot/TPlot.hxx
	@MOC@ $< -o $@
endif

if TFEL_QT5
moc_tplot.cxx : @top_srcdir@/tplot/include/TFEL/Plot/TPlot.hxx
	@QT_MOC@ $< -o $@
endif

tplot_LDFLAGS = -export-dynamic

if TFEL_QT4
tplot_LDADD   = -L$(top_builddir)/src/Plot           \
		-lTFELGnuplotInterpreter -lTFELPlot  \
		@QT4_LIBS@                           \
		-L@TFELHOME@/lib                     \
		-lTFELMathKriging                    \
		-lTFELMathParser                     \
		-lTFELMath                           \
		-lTFELUtilities                      \
		-lTFELSystem                         \
		-lTFELException
endif

if TFEL_QT5
tplot_LDADD   = -L$(top_builddir)/src/Plot           \
		-lTFELGnuplotInterpreter -lTFELPlot  \
		@QT_LIBS@                            \
		-L@TFELHOME@/lib                     \
		-lTFELMathKriging                    \
		-lTFELMathParser                     \
		-lTFELMath                           \
		-lTFELUtilities                      \
		-lTFELSystem                         \
		-lTFELException
endif

clean-local:
	$(RM) $(nodist_tplot_SOURCES)

EXTERNAL_RESOURCES=

if TFEL_QT4
resources.cxx : resources.qrc $(EXTERNAL_RESOURCES)
	@RCC@ @srcdir@/resources.qrc -o resources.cxx
endif

if TFEL_QT5
resources.cxx : resources.qrc $(EXTERNAL_RESOURCES)
	@QT_RCC@ @srcdir@/resources.qrc -o resources.cxx
endif

EXTRA_DIST=resources.qrc         \
	   $(EXTERNAL_RESOURCES) \
	   CMakeLists.txt
