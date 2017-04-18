## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_builddir)/tfel-plot-config/ \
	      -I$(top_srcdir)/include             \
	     `@TFELCONFIG@ --includes --cppflags`

bin_PROGRAMS = tfel-plot-config
tfel_plot_config_SOURCES = tfel-plot-config.cxx               
EXTRA_DIST = CMakeLists.txt
