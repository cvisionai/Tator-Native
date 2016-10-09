# Find the sources for gui
file( GLOB GUI_SOURCES "*.cc" )

# Add gui executable
if( WIN32 )
  add_executable( fish_gui WIN32 ${GUI_SOURCES} )
  target_link_libraries(
    fish_gui
    ${OpenCV_LIBS}
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    )
  set_target_properties( 
    fish_gui 
    PROPERTIES 
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS" 
    )
elseif( APPLE or UNIX )
  add_executable( fish_gui ${GUI_SOURCES} )
  target_link_libraries(
    fish_gui
    ${OpenCV_LIBS}
    Qt5::Widgets
    ${OPENGL_LIBRARIES}
    )
endif()

# Add install target 
install( 
  TARGETS fish_gui 
  DESTINATION . 
  )

