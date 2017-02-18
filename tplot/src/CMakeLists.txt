if(enable-qt4)
  include(${QT_USE_FILE})
  add_definitions(${QT_DEFINITIONS})
endif(enable-qt4)
include_directories("${PROJECT_SOURCE_DIR}/tplot/include")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${TFEL_INCLUDE_PATH}")

link_directories("${PROJECT_BINARY_DIR}/src/Plot")
link_directories("${TFEL_LIBRARY_PATH}")

qt_sources(TPlot_SOURCES 
  "${PROJECT_SOURCE_DIR}/tplot/include/TFEL/Plot" TPlot)
tfel_plot_executable(tplot
  tplot.cxx
  ${TPlot_SOURCES})

if(enable-qt4)
  target_link_libraries(tplot 
    TFELGnuplotInterpreter
    TFELPlot
    TFELMathParser
    TFELMath
    TFELUtilities
    TFELSystem
    TFELException
    ${QT_LIBRARIES})
endif(enable-qt4)
if(enable-qt5)
  target_link_libraries(tplot 
    TFELGnuplotInterpreter
    TFELPlot
    TFELMathParser
    TFELMath
    TFELUtilities
    TFELSystem
    TFELException
    Qt5::Widgets
    Qt5::PrintSupport)
endif(enable-qt5)
