
install(FILES default.global DESTINATION .)
install(FILES default.species DESTINATION .)
install(FILES default.colors DESTINATION .)
if(WIN32)
  if( ${BUILD_DB_UPLOADER} )
    install(FILES default.output_database DESTINATION .)
  endif()
endif()

