# --- Qt5 ---
find_package( Qt5 REQUIRED COMPONENTS 
  Core Gui Widgets Network Multimedia PrintSupport )
if( NOT APPLE )
  find_package( Qt5DBus REQUIRED )
endif()
find_package( Qt5Test REQUIRED )
get_target_property(QT5_WIDGETS_LOCATION Qt5::Widgets LOCATION )
message( STATUS "Found Qt5 at ${QT5_WIDGETS_LOCATION}" )
if( WIN32 )
  set( QT_THIRD_PARTY_LIBS
    "${_qt5Widgets_install_prefix}/lib/qtpcre.lib"
    "${_qt5Widgets_install_prefix}/lib/qtfreetype.lib"
    "${_qt5Widgets_install_prefix}/lib/qtpng.lib"
    "${_qt5Widgets_install_prefix}/lib/qtharfbuzzng.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5PlatformSupport.lib"
    "${_qt5Widgets_install_prefix}/plugins/platforms/qwindows.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qdds.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qgif.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qicns.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qico.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qjpeg.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qsvg.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qtga.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qtiff.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qwbmp.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qwebp.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5Svg.lib"
    Iphlpapi
    )
elseif( APPLE )
elseif( UNIX )
endif() 

# --- Boost ---
set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_STATIC_RUNTIME ON )
find_package( Boost REQUIRED COMPONENTS filesystem system )

# --- OpenCV ---
find_package( OpenCV REQUIRED )
if( WIN32 )
  file( GLOB_RECURSE OpenCV_SHARED_LIBS "${OpenCV_LIB_PATH}/../bin/*.dll" )
endif()

# --- OpenGL ---
find_package( OpenGL REQUIRED )

# --- Doxygen ---
find_package( Doxygen )
if( NOT DOXYGEN_FOUND )
  message( WARNING "Could not find Doxygen.  Documentation target will be unavailable." )
endif()

# --- LATEX ---
find_package( LATEX )
if( NOT LATEX_FOUND )
  message( WARNING "Could not find LATEX.  Documentation target will be unavailable." )
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

# --- Mac ---
elseif( APPLE )
  find_library( COCOA_LIBRARY Cocoa )
  message( STATUS "Found Cocoa at ${COCOA_LIBRARY}" )
  find_library( CARBON_LIBRARY Carbon )
  message( STATUS "Found Carbon at ${CARBON_LIBRARY}" )
  find_library( IOKIT_LIBRARY IOKit )
  message( STATUS "Found IOKit at ${IOKIT_LIBRARY}" )
  find_package( Cups REQUIRED )
  set( APPLE_LIBRARIES
    ${COCOA_LIBRARY}
    ${CARBON_LIBRARY}
    ${IOKIT_LIBRARY}
    ${CUPS_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES}
    )

# --- UNIX ---
elseif( UNIX )
  find_package( X11 REQUIRED )
  find_package( Threads REQUIRED )
  set( UNIX_LIBRARIES
    ${CMAKE_THREAD_LIBS_INIT}
    xcb
    X11-xcb
    ${X11_LIBRARIES}
    )
endif()
