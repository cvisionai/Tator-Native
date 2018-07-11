# --- Qt5 ---
find_package( Qt5 REQUIRED COMPONENTS 
  Core Gui Widgets PrintSupport OpenGL Sql )
if( NOT APPLE )
  find_package( Qt5 REQUIRED COMPONENTS DBus )
endif()
find_package( Qt5 REQUIRED COMPONENTS Test )
get_target_property(QT5_WIDGETS_LOCATION Qt5::Widgets LOCATION )
message( STATUS "Found Qt5 at ${QT5_WIDGETS_LOCATION}" )
if( WIN32 )
  set( QT_THIRD_PARTY_LIBS
    "${_qt5Widgets_install_prefix}/lib/qtpcre2.lib"
    "${_qt5Widgets_install_prefix}/lib/qtfreetype.lib"
    "${_qt5Widgets_install_prefix}/lib/qtlibpng.lib"
    "${_qt5Widgets_install_prefix}/lib/qtharfbuzz.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5FontDatabaseSupport.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5EventDispatcherSupport.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5ThemeSupport.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5AccessibilitySupport.lib"
    "${_qt5Widgets_install_prefix}/plugins/platforms/qwindows.lib"
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
    "${_qt5Widgets_install_prefix}/plugins/sqldrivers/qsqlodbc.lib"
    )
elseif( APPLE )
elseif( UNIX )
endif() 

# --- Boost ---
set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_STATIC_RUNTIME ON )
find_package( Boost REQUIRED COMPONENTS filesystem system )

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
    Iphlpapi
    D3d9
    dxva2
    evr
    mf
    mfplat
    mfplay
    mfreadwrite
    mfuuid
    dmoguids
    strmiids
    wmcodecdspuuid
    odbc32
    version
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
