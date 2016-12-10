# Add documentation target
if( ${DOXYGEN_FOUND} STREQUAL "YES" )
  if( ${LATEX_FOUND} )
    configure_file( ${FISHDET_DOC_DIR}/manual.md.in ${FISHDET_DOC_DIR}/manual.md )
    add_custom_target( doc
      ${DOXYGEN_EXECUTABLE} "${FISHDET_DOC_DIR}/doxyfile"
      COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/doxygen/latex/make.bat"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      COMMENT "Generating API documentation with Doxygen." VERBATIM
      )
    add_custom_target( manual
      ${DOXYGEN_EXECUTABLE} "${FISHDET_DOC_DIR}/doxyfile_manual"
      COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/doxygen/manual/latex/make.bat"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      COMMENT "Generating manual with Doxygen." VERBATIM
      )
  endif()
endif()

