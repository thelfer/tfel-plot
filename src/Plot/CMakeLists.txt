if(enable-qt4)
  include(${QT_USE_FILE})
  add_definitions(${QT_DEFINITIONS})
endif(enable-qt4)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${TFEL_INCLUDE_PATH}")

link_directories("${TFEL_LIBRARY_PATH}")
link_directories("${CMAKE_CURRENT_BINARY_DIR}")

configure_file(GetInstallPath-Prefix.hxx.in GetInstallPath-Prefix.hxx @ONLY)

qt_add_resources(TFELPlotResources_RCC
  TFELPlotResources.qrc)
if(MSC)
set_property(SOURCE ${TFELPlotResources_RCC}
  PROPERTY COMPILE_FLAGS "-fvisibility=default")
endif(MSC)

qt_sources(TFELPlot_SOURCES  
  "${PROJECT_SOURCE_DIR}/include/TFEL/Plot"
  Graph
  Curve
  CurveBase
  EvaluatedCurve
  DataCurve
  CurveConfigurationDialogBase
  CurveConfigurationDialog
  GraphConfigurationDialog
  CurveConfigurationAction
  ThemeSelectionAction
  GraphCurveItem
  GraphTextItem
  GraphKeyItem
  GraphBorderItem
  ImportTextDataDialogBase
  ImportTextDataDialog
  ImportLicosResultsDialog)

tfel_plot_library(TFELPlot
  ${TFELPlot_SOURCES}
  GetInstallPath.cxx
  ArrowStyle.cxx
  GraphCoordinates.cxx
  GraphTheme.cxx
  StandardGraphTheme.cxx
  ElegantGraphTheme.cxx
  CEAGraphTheme.cxx
  MFrontGraphTheme.cxx
  ThemeManager.cxx
  TextDataReader.cxx
  ImportLicosCurveDialog.cxx
  ImportAlcyoneCurveDialog.cxx
  ImportMTestResultsDialog.cxx
  ${TFELPlotResources_RCC})

if(enable-qt4)
  target_link_libraries(TFELPlot
    TFELMathParser
    TFELMath
    TFELUtilities
    TFELSystem
    TFELException
    ${QT_LIBRARIES})
endif(enable-qt4)
if(enable-qt5)
  target_link_libraries(TFELPlot
    TFELMathParser
    TFELMath
    TFELUtilities
    TFELSystem
    TFELException
    Qt5::Widgets
    Qt5::Svg
    Qt5::PrintSupport
    Qt5::Xml)
endif(enable-qt5)

qt_sources(TFELGnuplotInterpreter_SOURCES  
  "${PROJECT_SOURCE_DIR}/include/TFEL/Plot"
  GnuplotInterpreterBase
  GnuplotInterpreter
  GraphShell)

tfel_plot_library(TFELGnuplotInterpreter
  GnuplotPlotInterpreter.cxx
  GnuplotSetInterpreter.cxx
  GnuplotUnSetInterpreter.cxx
  GnuplotImportInterpreter.cxx
  GnuplotFitInterpreter.cxx
  GnuplotKrigingInterpreter.cxx
  ${TFELGnuplotInterpreter_SOURCES})
target_link_libraries(TFELGnuplotInterpreter
  TFELPlot
  TFELMathKriging
  TFELMathParser
  TFELMath
  TFELUtilities
  TFELSystem
  TFELException
  ${QT_LIBRARIES})
