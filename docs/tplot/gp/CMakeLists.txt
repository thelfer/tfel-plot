macro(install_tplot_gp_desc file)
  # if(HAVE_PANDOC)
  #   ADD_CUSTOM_COMMAND(
  #     OUTPUT    ${file}.html
  #     DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md
  #     DEPENDS   ${PROJECT_SOURCE_DIR}/docs/web/tplot-template.html
  #     DEPENDS   ${PROJECT_SOURCE_DIR}/docs/web/css/main.css
  #     COMMAND   ${PANDOC}
  #     ARGS      -f markdown-markdown_in_html_blocks+tex_math_single_backslash
  #     ARGS      --mathjax
  #     ARGS      ${ARGN}
  #     ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md -o ${file}.html)
  #   add_custom_target(tplot-${file}-html ALL DEPENDS ${file}.html)
  #   add_dependencies(website tplot-${file}-html)
  #   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}.html
  #     DESTINATION share/doc/tfel/web/tplot
  #     COMPONENT website)
  # endif(HAVE_PANDOC)
  install(FILES ${file}.md DESTINATION share/doc/tfel-plot/tplot/gp
    COMPONENT tplot)
endmacro(install_tplot_gp_desc file)

install_tplot_gp_desc(include)
