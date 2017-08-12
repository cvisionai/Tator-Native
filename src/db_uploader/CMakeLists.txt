include_directories(
  "${CMAKE_CURRENT_BINARY_DIR}/../common/common_autogen/include"
  "${CMAKE_CURRENT_BINARY_DIR}/../common"
  "../common"
)

# Add db_uploader executable
if( WIN32 )
  file( COPY ${FISHDET_ICON} DESTINATION . )
  add_executable( db_uploader WIN32 
    database_info.cc
    mainwindow.cc
    main.cc
    ../image_annotator/image_annotation.cc
    db_uploader.rc
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

