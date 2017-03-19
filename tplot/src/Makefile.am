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

bin_PROGRAMS  = tplot tplot-gui
tplot_SOURCES = tplot.cxx
tplot_gui_SOURCES = tplot-gui.cxx  \
	      	    TPlot.cxx      \
		    resources.cxx
if TPLOT_CLI
tplot_SOURCES += CLIHandler.cxx
endif

nodist_tplot_gui_SOURCES  = moc_tplot-gui.cxx
if TPLOT_CLI
nodist_tplot_SOURCES = moc_CLIHandler.cxx 
endif

if TFEL_QT4
moc_tplot-gui.cxx : @top_srcdir@/tplot/include/TFEL/Plot/TPlot.hxx
	@MOC@ $< -o $@
moc_%.cxx : @top_srcdir@/tplot/include/TFEL/Plot/%.hxx
	@MOC@ $< -o $@	
endif

if TFEL_QT5
moc_tplot-gui.cxx : @top_srcdir@/tplot/include/TFEL/Plot/TPlot.hxx
	@QT_MOC@ $< -o $@
moc_%.cxx : @top_srcdir@/tplot/include/TFEL/Plot/%.hxx
	@QT_MOC@ $< -o $@	
endif TFEL_QT5

tplot_gui_LDFLAGS = -export-dynamic

if TFEL_QT4
tplot_LDADD     = @QT4_LIBS@ 
if TPLOT_CLI
tplot_LDADD += -lreadline
endif
tplot_gui_LDADD = -L$(top_builddir)/src/Plot           \
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
tplot_LDADD     = @QT_LIBS@ 
if TPLOT_CLI
tplot_LDADD += -lreadline
endif
tplot_gui_LDADD = -L$(top_builddir)/src/Plot           \
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
	$(RM) $(nodist_tplot_gui_SOURCES) $(nodist_tplot_SOURCES)

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
