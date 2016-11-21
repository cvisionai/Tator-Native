file( GLOB COMMON_SOURCES "common/*.cc" )

add_subdirectory( image_annotator )
add_subdirectory( video_annotator )
add_subdirectory( video_slicer )

# Add install target for OpenCV shared libs
if( WIN32 )
  install( FILES ${OpenCV_SHARED_LIBS} DESTINATION . )
endif()
