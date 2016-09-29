
# Find the sources for gui
file( GLOB GUI_SOURCES "gui/*.cc" )

# AUTOMOC doesn't find all the headers, so we give it some help here
qt_wrap_cpp( fish_gui
  "${FISHDET_INCLUDE_DIR}/gui/mainwindow.h"
  "${FISHDET_INCLUDE_DIR}/gui/navigatorwidget.h"
  )

# Add an executable
add_executable( fish_gui ${GUI_SOURCES} )
target_link_libraries(
  fish_gui
  Qt5::Widgets
  )
