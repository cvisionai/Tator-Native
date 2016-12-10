# Find the sources for video slicer
file( GLOB VIDEO_SLICER_SOURCES "*.cc" )

# Add video slicer executable
if( WIN32 )
  file( COPY ${FISHDET_ICON} DESTINATION . )
  file( GLOB VIDEO_SLICER_RESOURCES "*.rc" )
  add_executable( video_slicer WIN32 
    ${VIDEO_SLICER_SOURCES} 
    ${VIDEO_SLICER_RESOURCES} 
    ${FISHDET_RES_FILE}
    )
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
elseif( APPLE )
  add_executable( video_slicer MACOSX_BUNDLE
    ${VIDEO_SLICER_SOURCES} 
    ${FISHDET_ICON_FULL}
    ${FISHDET_RES_FILE}
    )
  set_target_properties( video_slicer PROPERTIES
    MACOSX_BUNDLE_ICON_FILE ${FISHDET_ICON}
    OUTPUT_NAME "Video Slicer" )
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
elseif( UNIX )
  add_executable( video_slicer
    ${VIDEO_SLICER_SOURCES}
    ${FISHDET_RES_FILE}
    )
  target_link_libraries(
    video_slicer
    dl
    ${UNIX_LIBRARIES}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::PrintSupport
    Qt5::QXcbIntegrationPlugin
    ${QT_THIRD_PARTY_LIBS}
    Qt5::DBus
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
    )
endif()

# Add install target
install( 
  TARGETS video_slicer 
  DESTINATION . 
  )
