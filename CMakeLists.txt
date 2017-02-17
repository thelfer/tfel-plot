include(cmake/modules/tfel-graphics.cmake)
tfel_graphics_project(tfel-graphics 0 1-dev)
cmake_minimum_required(VERSION 2.4)

include(cmake/modules/autotools.cmake)

#compiler options
include(cmake/modules/compiler.cmake)
#find the tfel package
include(cmake/modules/tfel.cmake)
# qt support
include(cmake/modules/qt.cmake)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tplot)
