
# Find the sources for gui
file( GLOB GUI_SOURCES "gui/*.cc" )

# Add an executable
if( WIN32 )
  add_executable( fish_gui WIN32 ${GUI_SOURCES} )
  target_link_libraries(
    fish_gui
    ${OpenCV_LIBS}
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    "${_qt5Widgets_install_prefix}/lib/qtpcre.lib"
    "${_qt5Widgets_install_prefix}/lib/qtfreetype.lib"
    "${_qt5Widgets_install_prefix}/lib/qtpng.lib"
    "${_qt5Widgets_install_prefix}/lib/qtharfbuzzng.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5PlatformSupport.lib"
    "${_qt5Widgets_install_prefix}/plugins/platforms/qwindows.lib"
    ${OPENGL_LIBRARIES}
    )
  set_target_properties( 
    fish_gui 
    PROPERTIES 
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS" 
    )
endif()
