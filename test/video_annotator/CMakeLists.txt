set( TEST_VIDEO_ANNOTATOR_SOURCES test_video_annotator.cc )
set( VIDEO_ANNOTATOR_SOURCES
  "../../src/video_annotator/document.cc"
  "../../src/video_annotator/fish.cc"
  "../../src/video_annotator/mainwindow.cc"
  "../../src/video_annotator/mainwindowbuttons.cc"
  "../../src/video_annotator/mainwindowmenus.cc"
  "../../src/video_annotator/navigatorwidget.cc"
  "../../src/video_annotator/player.cc"
  "../../src/common/rect.cc"
  "../../src/common/serialization.cc"
  )
include_directories( 
  "../../include/fish_detector/video_annotator"
  "../../include/fish_detector/common"
  )
if( WIN32 )
  add_executable( test_video_annotator WIN32 
    ${TEST_VIDEO_ANNOTATOR_SOURCES} 
    ${VIDEO_ANNOTATOR_SOURCES}
    )
  target_link_libraries( 
    test_video_annotator 
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
    )
  set_tests_properties( 
    test_video_annotator PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
endif()
