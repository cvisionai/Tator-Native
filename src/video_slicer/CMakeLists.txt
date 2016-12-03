# Find the sources for video slicer
file( GLOB VIDEO_SLICER_SOURCES "*.cc" )

# Add video slicer executable
if( WIN32 )
  add_executable( video_slicer WIN32 ${VIDEO_SLICER_SOURCES} )
  target_link_libraries(
    video_slicer
    ${OpenCV_LIBS}
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    )
  set_target_properties( 
    video_slicer
    PROPERTIES 
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS" 
    )
else()
  add_executable( video_slicer MACOSX_BUNDLE
    ${VIDEO_SLICER_SOURCES} ${FISHDET_ICON_FULL} )
  set_target_properties( video_slicer PROPERTIES
    MACOSX_BUNDLE_ICON_FILE ${FISHDET_ICON} )
  set_source_files_properties( ${FISHDET_ICON_FULL} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources" )
  target_link_libraries(
    video_slicer
    ${OpenCV_LIBS}
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::PrintSupport
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    )
endif()

# Add install target
install( 
  TARGETS video_slicer 
  DESTINATION . 
  )
