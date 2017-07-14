# Find the sources for video annotator
file( GLOB VIDEO_ANNOTATOR_SOURCES "*.cc" )

# Add video annotator executable
if( WIN32 )
  file( COPY ${FISHDET_ICON} DESTINATION . )
  file( GLOB VIDEO_ANNOTATOR_RESOURCES "*.rc" )
  add_executable( video_annotator WIN32 
    ${VIDEO_ANNOTATOR_SOURCES} 
    ${VIDEO_ANNOTATOR_RESOURCES}
    ${FISHDET_RES_FILE} 
    )
  target_link_libraries(
    video_annotator
    common
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    ${QT_THIRD_PARTY_LIBS}
    ${Boost_LIBRARIES}
    ${FFMPEG_LIBRARY_DIR}/avformat.lib
    ${FFMPEG_LIBRARY_DIR}/avcodec.lib
    ${FFMPEG_LIBRARY_DIR}/avutil.lib
    ${FFMPEG_LIBRARY_DIR}/swscale.lib
    )
  set_target_properties( 
    video_annotator 
    PROPERTIES 
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS" 
    )
  target_include_directories( video_annotator PUBLIC
    ${FFMPEG_INCLUDE_DIR}
    )
elseif( APPLE )
  add_executable( video_annotator MACOSX_BUNDLE
    ${VIDEO_ANNOTATOR_SOURCES} 
    ${FISHDET_ICON_FULL}
    ${FISHDET_RES_FILE} 
    )
  set_target_properties( video_annotator PROPERTIES
    MACOSX_BUNDLE_ICON_FILE ${FISHDET_ICON}
    OUTPUT_NAME "Video Annotator" )
  set_source_files_properties( ${FISHDET_ICON_FULL} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources" )
  target_link_libraries(
    video_annotator
    common
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::PrintSupport
    ${Boost_LIBRARIES}
    )
elseif( UNIX )
  add_executable( video_annotator
    ${VIDEO_ANNOTATOR_SOURCES} 
    ${FISHDET_RES_FILE}
    )
  target_link_libraries(
    video_annotator
    dl
    common
    ${UNIX_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::PrintSupport
    ${QT_THIRD_PARTY_LIBS}
    Qt5::DBus
    ${Boost_LIBRARIES}
    avformat
    avcodec
    avutil
    swscale
    )
endif()

# Add install target 
install( 
  TARGETS video_annotator 
  DESTINATION . 
  )

