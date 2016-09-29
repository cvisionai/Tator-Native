
# Find the sources for gui
file( GLOB GUI_SOURCES "gui/*.cc" )

# Add an executable
add_executable( fish_gui ${GUI_SOURCES} )
target_link_libraries(
  fish_gui
  Qt5::Widgets
  )
