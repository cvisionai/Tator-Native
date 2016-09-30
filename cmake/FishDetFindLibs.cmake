# --- Qt5 ---
find_package( Qt5Widgets )
if( NOT Qt5Widgets_FOUND )
  message( FATAL_ERROR "Could not find Qt5Widgets.  Build cannot continue." )
endif()
find_package( Qt5Core )
if( NOT Qt5Core_FOUND )
  message( FATAL_ERROR "Could not find Qt5Core.  Build cannot continue." )
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

