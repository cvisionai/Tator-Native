# --- Qt5 ---
find_package( Qt5Widgets )
if( NOT Qt5Widgets_FOUND )
  message( FATAL_ERROR "Could not find Qt5Widgets.  Build cannot continue." )
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
