# Find the sources for video annotator
set( VIDEO_ANNOTATOR_SOURCES 
  "main.cc"
  "mainwindow.cc"
  "player.cc"
  "video_annotation.cc"
  "reassign_dialog.cc"
)
set( VIDEO_ANNOTATOR_RESOURCES
  "video_annotator.rc"
)

include_directories(
  "${CMAKE_CURRENT_BINARY_DIR}/../common/common_autogen/include"
  "${CMAKE_CURRENT_BINARY_DIR}/../common/common_autogen/include_${CMAKE_BUILD_TYPE}"
  "${CMAKE_CURRENT_BINARY_DIR}/../common"
  "../common"
)

# Add video annotator executable
if( WIN32 )
  file( COPY ${TATOR_ICON} DESTINATION . )
  add_executable( video_annotator WIN32 
    ${VIDEO_ANNOTATOR_SOURCES} 
    ${VIDEO_ANNOTATOR_RESOURCES}
    ${TATOR_RES_FILE} 
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
    ${TATOR_ICON_FULL}
    ${TATOR_RES_FILE} 
    )
  set_target_properties( video_annotator PROPERTIES
    MACOSX_BUNDLE_ICON_FILE ${TATOR_ICON}
    OUTPUT_NAME "Video Annotator" )
  set_source_files_properties( ${TATOR_ICON_FULL} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources" )
  target_link_libraries(
    video_annotator
    common
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::PrintSupport
    ${Boost_LIBRARIES}
    /usr/local/Cellar/ffmpeg/3.4.2/lib/libavformat.dylib
    /usr/local/Cellar/ffmpeg/3.4.2/lib/libavcodec.dylib
    /usr/local/Cellar/ffmpeg/3.4.2/lib/libavutil.dylib
    /usr/local/Cellar/ffmpeg/3.4.2/lib/libswscale.dylib
    )
elseif( UNIX )
  add_executable( video_annotator
    ${VIDEO_ANNOTATOR_SOURCES} 
    ${TATOR_RES_FILE}
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

