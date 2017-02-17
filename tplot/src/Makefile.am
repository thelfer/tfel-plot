## Makefile.am -- Process this file with automake to produce a Makefile.in file.

localedir = $(datadir)/locale
AM_CPPFLAGS = -I$(top_srcdir)/tplot/include        \
	      -I$(top_srcdir)/include              \
	      @QT4_CFLAGS@                         \
	      `@TFELCONFIG@ --includes --cppflags` \
	      -DLOCALEDIR=\"$(localedir)\"

bin_PROGRAMS  = tplot
tplot_SOURCES = tplot.cxx     \
		TPlot.cxx     \
		resources.cxx
nodist_tplot_SOURCES = moc_tplot.cxx 

moc_tplot.cxx : @top_srcdir@/tplot/include/TFEL/Plot/TPlot.hxx
	@MOC@ $< -o $@

tplot_LDFLAGS = -export-dynamic
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

clean-local:
	$(RM) $(nodist_tplot_SOURCES)

EXTERNAL_RESOURCES=

resources.cxx : resources.qrc $(EXTERNAL_RESOURCES)
	@RCC@ @srcdir@/resources.qrc -o resources.cxx

EXTRA_DIST=resources.qrc         \
	   $(EXTERNAL_RESOURCES) \
	   CMakeLists.txt
