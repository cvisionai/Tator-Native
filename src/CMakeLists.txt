add_subdirectory( common )
add_subdirectory( image_annotator )
add_subdirectory( video_annotator )
add_subdirectory( video_slicer )
add_subdirectory( db_uploader )

# Add install target for OpenCV shared libs
if( WIN32 )
  install( FILES ${OpenCV_SHARED_LIBS} DESTINATION . )
  file( GLOB FFMPEG_DLLS ${FFMPEG_LIBRARY_DIR}/*.dll )
  install( FILES ${FFMPEG_DLLS} DESTINATION . )
endif()
