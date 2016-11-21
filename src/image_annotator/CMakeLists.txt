# Find the sources for image annotator
file( GLOB IMAGE_ANNOTATOR_SOURCES "*.cc" )

# Add image_annotator executable
if( WIN32 )
  add_executable( image_annotator WIN32 
    ${IMAGE_ANNOTATOR_SOURCES} 
    ${COMMON_SOURCES} 
    )
  target_link_libraries(
    image_annotator
    ${OpenCV_LIBS}
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    )
  set_target_properties( 
    image_annotator 
    PROPERTIES 
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS" 
    )
elseif( APPLE )
  add_executable( image_annotator ${IMAGE_ANNOTATOR_SOURCES} )
  target_link_libraries(
    image_annotator
    ${OpenCV_LIBS}
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::PrintSupport
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    )
endif()

# Add install target 
install( 
  TARGETS image_annotator 
  DESTINATION . 
  )

