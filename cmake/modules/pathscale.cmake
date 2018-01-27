include(cmake/modules/compiler.cmake)
include(cmake/modules/common-compiler-flags.cmake)

set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(OPTIMISATION_FLAGS "-O3 -DNDEBUG ${OPTIMISATION_FLAGS}")
endif(CMAKE_BUILD_TYPE STREQUAL "Release")
