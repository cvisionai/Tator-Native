# Find the sources for image annotator
file( GLOB IMAGE_ANNOTATOR_SOURCES "*.cc" )

# Add image_annotator executable
if( WIN32 )
  file( COPY ${FISHDET_ICON} DESTINATION . )
  file( GLOB IMAGE_ANNOTATOR_RESOURCES "*.rc" )
  add_executable( image_annotator WIN32 
    ${IMAGE_ANNOTATOR_SOURCES} ${IMAGE_ANNOTATOR_RESOURCES} 
    )
  target_link_libraries(
    image_annotator
    common
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
  set_target_properties( 
    image_annotator 
    PROPERTIES 
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS" 
    )
elseif( APPLE )
  add_executable( image_annotator MACOSX_BUNDLE
    ${IMAGE_ANNOTATOR_SOURCES} ${FISHDET_ICON_FULL} )
  set_target_properties( image_annotator PROPERTIES
    MACOSX_BUNDLE_ICON_FILE ${FISHDET_ICON}
    OUTPUT_NAME "Image Annotator" 
  )
  set_source_files_properties( ${FISHDET_ICON_FULL} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources" )
  target_link_libraries(
    image_annotator
    common
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::PrintSupport
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
elseif( UNIX )
  add_executable( image_annotator
    ${IMAGE_ANNOTATOR_SOURCES} )
  target_link_libraries(
    image_annotator
    dl
    common
    ${UNIX_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::QXcbIntegrationPlugin
    ${QT_THIRD_PARTY_LIBS}
    Qt5::DBus
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
endif()

# Add install target 
install( 
  TARGETS image_annotator 
  DESTINATION . 
  )

