include_directories("${TFEL_INCLUDE_PATH}")
include_directories("${PROJECT_SOURCE_DIR}/include")

link_directories("${TFEL_LIBRARY_PATH}")
link_directories("${PROJECT_BINARY_DIR}/src/Plot")

# a simple macro
macro(tests_plot test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELPlot>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg} TFELGnuplotInterpreter TFELPlot TFELTests)
endmacro(tests_plot)

tests_plot(GnuplotInterpreterTest)
