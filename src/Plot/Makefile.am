## Makefile.am -- Process this file with automake to produce a Makefile.in file.

AM_CPPFLAGS = -I$(top_builddir)/src/Plot          \
	      -I$(top_srcdir)/include             \
	      $(QT_CXXFLAGS)                      \
	      `$(TFELCONFIG) --includes --cppflags`

lib_LTLIBRARIES = libTFELPlot.la               \
		  libTFELGnuplotInterpreter.la

libTFELPlot_la_SOURCES = GetInstallPath.cxx               \
			 ArrowStyle.cxx                   \
			 Curve.cxx                        \
			 CurveBase.cxx                    \
			 DataCurve.cxx	                  \
			 EvaluatedCurve.cxx	          \
			 GraphCoordinates.cxx             \
			 GraphTheme.cxx	                  \
			 StandardGraphTheme.cxx           \
			 ElegantGraphTheme.cxx            \
			 CEAGraphTheme.cxx                \
			 MFrontGraphTheme.cxx             \
			 ThemeManager.cxx                 \
			 ThemeSelectionAction.cxx         \
			 CurveConfigurationDialogBase.cxx \
			 CurveConfigurationDialog.cxx     \
			 GraphConfigurationDialog.cxx     \
			 CurveConfigurationAction.cxx     \
			 GraphCurveItem.cxx               \
			 GraphTextItem.cxx                \
			 GraphKeyItem.cxx                 \
			 GraphBorderItem.cxx              \
			 TextDataReader.cxx               \
			 ImportTextDataDialogBase.cxx     \
			 ImportTextDataDialog.cxx         \
			 ImportLicosCurveDialog.cxx       \
			 ImportAlcyoneCurveDialog.cxx     \
			 ImportLicosResultsDialog.cxx     \
			 ImportMTestResultsDialog.cxx     \
			 InitRessources.cxx               \
			 Graph.cxx
libTFELPlot_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELPlot_EXPORTS

nodist_libTFELPlot_la_SOURCES = moc_Graph.cxx                        \
				moc_Curve.cxx                        \
				moc_CurveBase.cxx                    \
				moc_EvaluatedCurve.cxx               \
				moc_DataCurve.cxx                    \
				moc_GraphConfigurationDialog.cxx     \
				moc_ThemeSelectionAction.cxx         \
				moc_CurveConfigurationDialogBase.cxx \
				moc_CurveConfigurationDialog.cxx     \
				moc_CurveConfigurationAction.cxx     \
				moc_GraphCurveItem.cxx               \
				moc_GraphKeyItem.cxx                 \
				moc_GraphTextItem.cxx                \
				moc_GraphBorderItem.cxx              \
				moc_ImportTextDataDialog.cxx     \
				moc_ImportTextDataDialogBase.cxx     \
				moc_ImportLicosResultsDialog.cxx     \
				TFELPlotResources.cxx

libTFELGnuplotInterpreter_la_SOURCES = GraphShell.cxx                 \
				       GnuplotInterpreter.cxx         \
			               GnuplotInterpreterBase.cxx     \
			               GnuplotPlotInterpreter.cxx     \
			               GnuplotSetInterpreter.cxx      \
			               GnuplotUnSetInterpreter.cxx    \
			               GnuplotImportInterpreter.cxx   \
			               GnuplotFitInterpreter.cxx      \
			               GnuplotKrigingInterpreter.cxx

libTFELGnuplotInterpreter_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELGnuplotInterpreter_EXPORTS
nodist_libTFELGnuplotInterpreter_la_SOURCES = moc_GnuplotInterpreterBase.cxx \
					      moc_GnuplotInterpreter.cxx     \
				       	      moc_GraphShell.cxx

libTFELGnuplotInterpreter_la_DEPENDENCIES = libTFELPlot.la

libTFELPlot_la_LIBADD = $(QT_LIBS)          \
			-L$(TFELHOME)/lib               \
			-lTFELMathParser                \
			-lTFELMath                      \
			-lTFELUtilities                 \
			-lTFELSystem                    \
			-lTFELUnicodeSupport            \
			-lTFELException

libTFELGnuplotInterpreter_la_LIBADD = libTFELPlot.la        \
			    	      $(QT_LIBS)             \
			    	      -L$(TFELHOME)/lib      \
			    	      -lTFELMathKriging     \
			    	      -lTFELMathParser      \
			    	      -lTFELMath            \
			    	      -lTFELUtilities       \
			    	      -lTFELSystem          \
			    	      -lTFELUnicodeSupport  \
			    	      -lTFELException

TFELGRAPHICSRESOURCES=images/CEABanner2012.svg   \
		      images/CEABanner2012-2.svg \
		      images/CEALogo2012.svg     \
		      images/mfront-en.svg

moc_%.cxx : $(top_srcdir)/include/TFEL/Plot/%.hxx
	$(QT_MOC) $< -o $@

TFELPlotResources.cxx : TFELPlotResources.qrc $(TFELGRAPHICSRESOURCES)
	$(QT_RCC) -name TFELPlotResources $(srcdir)/TFELPlotResources.qrc -o TFELPlotResources.cxx

clean-local:
	$(RM) $(nodist_libTFELPlot_la_SOURCES)
	$(RM) $(nodist_libTFELGnuplotInterpreter_la_SOURCES)

EXTRA_DIST=TFELPlotResources.qrc \
	   $(TFELGRAPHICSRESOURCES)  \
           CMakeLists.txt
