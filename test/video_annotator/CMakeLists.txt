set( TEST_VIDEO_ANNOTATOR_SOURCES test_video_annotator.cc )
set( VIDEO_ANNOTATOR_SOURCES
  "../../src/video_annotator/document.cc"
  "../../src/video_annotator/fish.cc"
  "../../src/video_annotator/mainwindow.cc"
  "../../src/video_annotator/mainwindowbuttons.cc"
  "../../src/video_annotator/mainwindowmenus.cc"
  "../../src/video_annotator/navigatorwidget.cc"
  "../../src/video_annotator/player.cc"
  )
include_directories( 
  "../../include/fish_annotator/video_annotator"
  "../../include/fish_annotator/common"
  )
if( WIN32 )
  add_executable( test_video_annotator WIN32 
    ${TEST_VIDEO_ANNOTATOR_SOURCES} 
    ${VIDEO_ANNOTATOR_SOURCES}
    )
  target_link_libraries( 
    test_video_annotator 
    common
    ${OpenCV_LIBS}
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    )
  set_target_properties(
    test_video_annotator
    PROPERTIES
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:CONSOLE"
    )
  add_test( 
    NAME test_video_annotator 
    COMMAND test_video_annotator 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Release"
    )
  set_tests_properties( 
    test_video_annotator PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
  file( COPY ${OpenCV_SHARED_LIBS} DESTINATION Release )
  file( COPY slow_motion_drop.mp4 DESTINATION Release )
elseif( APPLE )
  add_executable( test_video_annotator 
    ${TEST_VIDEO_ANNOTATOR_SOURCES} 
    ${VIDEO_ANNOTATOR_SOURCES}
    )
  target_link_libraries( 
    test_video_annotator 
    common
    ${OpenCV_LIBS}
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    )
  add_test( 
    NAME test_video_annotator 
    COMMAND test_video_annotator 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  set_tests_properties( 
    test_video_annotator PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
  file( COPY ${OpenCV_SHARED_LIBS} DESTINATION . )
  file( COPY slow_motion_drop.mp4 DESTINATION . )
endif()
