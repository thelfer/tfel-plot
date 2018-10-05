## Makefile.am -- Process this file with automake to produce a Makefile.in file.

localedir = $(datadir)/locale

AM_CPPFLAGS = -I$(top_srcdir)/tplot/include        \
	      -I$(top_srcdir)/include              \
	      $(QT_CXXFLAGS)                       \
	      `$(TFELCONFIG) --includes --cppflags`\
	      -DLOCALEDIR=\"$(localedir)\"

bin_PROGRAMS  = tplot tplot-gui
tplot_SOURCES = tplot-main.cxx
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

moc_tplot-gui.cxx : $(top_srcdir)/tplot/include/TFEL/Plot/TPlot.hxx
	$(QT_MOC) $< -o $@
moc_%.cxx : $(top_srcdir)/tplot/include/TFEL/Plot/%.hxx
	$(QT_MOC) $< -o $@

tplot_gui_LDFLAGS = -export-dynamic

tplot_LDADD     = $(QT_LIBS)

if TPLOT_CLI
tplot_LDADD += -lreadline
endif
tplot_gui_LDADD = $(top_builddir)/src/Plot/libTFELGnuplotInterpreter.la \
		  $(top_builddir)/src/Plot/libTFELPlot.la \
		  $(QT_LIBS)                            \
		  -L$(TFELHOME)/lib                     \
		  -lTFELMathKriging                    \
		  -lTFELMathParser                     \
		  -lTFELMath                           \
		  -lTFELUtilities                      \
		  -lTFELSystem                         \
		  -lTFELException

clean-local:
	$(RM) $(nodist_tplot_gui_SOURCES) $(nodist_tplot_SOURCES)

EXTERNAL_RESOURCES=

resources.cxx : resources.qrc $(EXTERNAL_RESOURCES)
	$(QT_RCC) $(srcdir)/resources.qrc -o resources.cxx

EXTRA_DIST=resources.qrc         \
	   $(EXTERNAL_RESOURCES) \
	   CMakeLists.txt
