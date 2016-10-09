# --- Qt5 ---
find_package( Qt5Widgets )
if( NOT Qt5Widgets_FOUND )
  message( FATAL_ERROR "Could not find Qt5Widgets.  Build cannot continue." )
endif()
find_package( Qt5Core )
if( NOT Qt5Core_FOUND )
  message( FATAL_ERROR "Could not find Qt5Core.  Build cannot continue." )
endif()
find_package( Qt5Gui )
if( NOT Qt5Gui_FOUND )
  message( FATAL_ERROR "Could not find Qt5Gui.  Build cannot continue." )
endif()
find_package( Qt5PrintSupport )
if( NOT Qt5PrintSupport_FOUND )
  message( FATAL_ERROR "Could not find Qt5PrintSupport.  Build cannot continue." )
endif()
if( WIN32 )
  set( QT_THIRD_PARTY_LIBS
    "${_qt5Widgets_install_prefix}/lib/qtpcre.lib"
    "${_qt5Widgets_install_prefix}/lib/qtfreetype.lib"
    "${_qt5Widgets_install_prefix}/lib/qtpng.lib"
    "${_qt5Widgets_install_prefix}/lib/qtharfbuzzng.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5PlatformSupport.lib"
    "${_qt5Widgets_install_prefix}/plugins/platforms/qwindows.lib"
    )
else()
  set( QT_THIRD_PARTY_LIBS
    "${_qt5Widgets_install_prefix}/lib/libqtpcre.a"
    "${_qt5Widgets_install_prefix}/lib/libqtfreetype.a"
    "${_qt5Widgets_install_prefix}/lib/libqtharfbuzzng.a"
    "${_qt5Widgets_install_prefix}/lib/libQt5PlatformSupport.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqcocoa.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqminimal.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqoffscreen.a"
    "${_qt5Widgets_install_prefix}/plugins/printsupport/libcocoaprintersupport.a"
    )
endif() 

# --- Boost ---
find_package( Boost )
if( NOT Boost_FOUND )
  message( FATAL_ERROR "Could not find Boost.  Build cannot continue." )
endif()

# --- OpenCV ---
find_package( OpenCV )
if( NOT OpenCV_FOUND )
  message( FATAL_ERROR "Could not find OpenCV.  Build cannot continue." )
endif()
if( WIN32 )
  file( GLOB_RECURSE OpenCV_SHARED_LIBS "${OpenCV_LIB_PATH}/../bin/*.dll" )
endif()

# --- OpenGL ---
find_package( OpenGL )
if( NOT OPENGL_FOUND )
  message( FATAL_ERROR "Could not find OpenGL.  Build cannot continue." )
endif()

# --- Windows ---
if( MSVC )
  set( WINDOWS_LIBRARIES
    winmm
    UxTheme
    ws2_32
    imm32
    Dwmapi
    )
endif()

# --- Mac ---
if( APPLE )
  find_library( COCOA_LIBRARY Cocoa )
  find_library( CARBON_LIBRARY Carbon )
  find_library( IOKIT_LIBRARY IOKit )
  find_package( Cups )
  if( NOT CUPS_FOUND )
    message (FATAL_ERROR "Could not find Cups.  Build cannot continue." )
  endif()
  find_package( ZLIB )
  if( NOT ZLIB_FOUND )
    message( FATAL_ERROR "Could not find zlib.  Build cannot continue." )
  endif()
  set( APPLE_LIBRARIES
    ${COCOA_LIBRARY}
    ${CARBON_LIBRARY}
    ${IOKIT_LIBRARY}
    ${CUPS_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES}
    )
endif()
