# Find the sources for video annotator
file( GLOB VIDEO_ANNOTATOR_SOURCES "*.cc" )

# Add video annotator executable
if( WIN32 )
  file( COPY ${FISHDET_ICON} DESTINATION . )
  file( GLOB VIDEO_ANNOTATOR_RESOURCES "*.rc" )
  add_executable( video_annotator WIN32 
    ${VIDEO_ANNOTATOR_SOURCES} 
    ${VIDEO_ANNOTATOR_RESOURCES}
    ${FISHDET_RES_FILE} 
    )
  target_link_libraries(
    video_annotator
    common
    ${OpenCV_LIBS}
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    Qt5::OpenGL
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
  set_target_properties( 
    video_annotator 
    PROPERTIES 
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS" 
    )
elseif( APPLE )
  add_executable( video_annotator MACOSX_BUNDLE
    ${VIDEO_ANNOTATOR_SOURCES} 
    ${FISHDET_ICON_FULL}
    ${FISHDET_RES_FILE} 
    )
  set_target_properties( video_annotator PROPERTIES
    MACOSX_BUNDLE_ICON_FILE ${FISHDET_ICON}
    OUTPUT_NAME "Video Annotator" )
  set_source_files_properties( ${FISHDET_ICON_FULL} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources" )
  target_link_libraries(
    video_annotator
    common
    ${OpenCV_LIBS}
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::PrintSupport
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
elseif( UNIX )
  add_executable( video_annotator
    ${VIDEO_ANNOTATOR_SOURCES} 
    ${FISHDET_RES_FILE}
    )
  target_link_libraries(
    video_annotator
    dl
    common
    ${OpenCV_LIBS}
    ${UNIX_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::PrintSupport
    ${QT_THIRD_PARTY_LIBS}
    Qt5::DBus
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
endif()

# Add install target 
install( 
  TARGETS video_annotator 
  DESTINATION . 
  )

