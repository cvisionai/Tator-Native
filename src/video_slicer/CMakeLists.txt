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
endif()

# Add install target
install( 
  TARGETS video_slicer 
  DESTINATION . 
  )
