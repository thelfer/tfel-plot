# find the tfel library
if(TFEL_INSTALL_PATH)
  set(TFELHOME "${TFEL_INSTALL_PATH}")
else(TFEL_INSTALL_PATH)
  set(TFELHOME $ENV{TFELHOME})
endif(TFEL_INSTALL_PATH)

if(LIB_SUFFIX)
  add_definitions("-DLIB_SUFFIX=\\\"\"${LIB_SUFFIX}\"\\\"")
endif(LIB_SUFFIX)

# type of architecture
find_program(MFRONT       mfront
  HINTS "${TFELHOME}/bin")
find_program(TFEL_CONFIG  tfel-config
  HINTS "${TFELHOME}/bin")

IF(NOT (TFEL_CONFIG AND MFRONT))
  MESSAGE(FATAL_ERROR "tfel not found")
ENDIF(NOT (TFEL_CONFIG AND MFRONT))

EXECUTE_PROCESS(COMMAND ${TFEL_CONFIG} "--cppflags"
  OUTPUT_VARIABLE TFEL_CPP_FLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND ${TFEL_CONFIG} "--include-path"
  OUTPUT_VARIABLE TFEL_INCLUDE_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND ${TFEL_CONFIG} "--library-path"
  OUTPUT_VARIABLE TFEL_LIBRARY_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

macro(find_tfel_library name)
  find_library(${name}
    NAMES ${name}
    HINTS ${TFEL_LIBRARY_PATH})
  if(NOT ${name})
    MESSAGE(FATAL_ERROR "${name} library not found")
  endif(NOT ${name})
endmacro(find_tfel_library name)

find_tfel_library(TFELTests)
find_tfel_library(TFELException)
find_tfel_library(TFELUtilities)
find_tfel_library(TFELMath)
find_tfel_library(TFELMaterial)

MESSAGE(STATUS "mfront        : ${MFRONT}")
MESSAGE(STATUS "tfel-config   : ${TFEL_CONFIG}")
MESSAGE(STATUS "tfel cppflags : ${TFEL_CPP_FLAGS}")
MESSAGE(STATUS "tfel include  : ${TFEL_INCLUDE_PATH}")
MESSAGE(STATUS "tfel libs     : ${TFEL_LIBRARY_PATH}")
MESSAGE(STATUS "TFELTests     : ${TFELTests}")
MESSAGE(STATUS "TFELTests     : ${TFELTests}")
MESSAGE(STATUS "TFELException : ${TFELException}")
MESSAGE(STATUS "TFELUtilities : ${TFELUtilities}")
MESSAGE(STATUS "TFELMath      : ${TFELMath}")	
MESSAGE(STATUS "TFELMaterial  : ${TFELMaterial}") 

set(COMPILER_CXXFLAGS "${COMPILER_CXXFLAGS} ${TFEL_CPP_FLAGS}")
