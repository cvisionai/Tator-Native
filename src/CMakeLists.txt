add_subdirectory( common )
add_subdirectory( image_annotator )
add_subdirectory( video_annotator )

if( WIN32 )
  add_subdirectory( db_uploader )
  file( GLOB FFMPEG_DLLS ${FFMPEG_LIBRARY_DIR}/*.dll )
  install( FILES ${FFMPEG_DLLS} DESTINATION . )
endif()
