include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_BINARY_DIR}/tfel-plot-config"
		    "${TFEL_INCLUDE_PATH}")
		  
configure_file(${PROJECT_SOURCE_DIR}/tfel-plot-config/tfel-plot-config.hxx.in
               ${PROJECT_BINARY_DIR}/tfel-plot-config/tfel-plot-config.hxx)

add_executable(tfel-plot-config tfel-plot-config.cxx)
if(WIN32 AND enable-static)
  set_target_properties(tfel-plot-config PROPERTIES COMPILE_FLAGS "-DTFEL_STATIC_BUILD")
  set_target_properties(tfel-plot-config PROPERTIES LINK_FLAGS "-static-libgcc -static") 
  set_target_properties(tfel-plot-config PROPERTIES LINK_SEARCH_END_STATIC ON) 
endif(WIN32 AND enable-static)

install(TARGETS tfel-plot-config DESTINATION bin)
