# Find the sources for image annotator
file( GLOB DB_UPLOADER_SOURCES "*.cc" )

# Add db_uploader executable
if( WIN32 )
  file( COPY ${FISHDET_ICON} DESTINATION . )
  file( GLOB DB_UPLOADER_RESOURCES "*.rc" )
  add_executable( db_uploader WIN32 
    ${DB_UPLOADER_SOURCES} 
    ${DB_UPLOADER_RESOURCES} 
    ${FISHDET_RES_FILE} 
    )
  target_link_libraries(
    db_uploader
    common
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    Qt5::Gui
    ${QT_THIRD_PARTY_LIBS}
    Qt5::Sql
    ${Boost_LIBRARIES}
    )
  set_target_properties( 
    db_uploader
    PROPERTIES 
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS" 
    )
endif()

# Add install target 
install( 
  TARGETS db_uploader
  DESTINATION . 
  )

