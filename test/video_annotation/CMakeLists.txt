set( TEST_VIDEO_ANNOTATION_SOURCES test_video_annotation.cc )
set( VIDEO_ANNOTATION_SOURCES
  "../../src/video_annotator/video_annotation.cc"
  )
include_directories( 
  "../../include/fish_annotator/common"
  "../../include/fish_annotator/video_annotator"
  )
if( WIN32 )
  add_executable( test_video_annotation WIN32 
    ${TEST_VIDEO_ANNOTATION_SOURCES} 
    ${VIDEO_ANNOTATION_SOURCES}
    )
  target_link_libraries(
    test_video_annotation
    common
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    ${Boost_LIBRARIES}
  )
  set_target_properties(
    test_video_annotation
    PROPERTIES
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:CONSOLE"
    )
  add_test( 
    NAME test_video_annotation 
    COMMAND test_video_annotation 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Release"
    )
  set_tests_properties( 
    test_video_annotation PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
elseif( APPLE )
  add_executable( test_video_annotation
    ${TEST_VIDEO_ANNOTATION_SOURCES} 
    ${VIDEO_ANNOTATION_SOURCES}
    )
  target_link_libraries( 
    test_video_annotation 
    common
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    )
  add_test( 
    NAME test_video_annotation 
    COMMAND test_video_annotation 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  set_tests_properties( 
    test_video_annotation PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
elseif( UNIX )
  add_executable( test_video_annotation
    ${TEST_VIDEO_ANNOTATION_SOURCES} 
    ${VIDEO_ANNOTATION_SOURCES}
    )
  target_link_libraries( 
    test_video_annotation 
    dl
    common
    ${UNIX_LIBRARIES}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::PrintSupport
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    Qt5::DBus
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    )
  add_test( 
    NAME test_video_annotation 
    COMMAND test_video_annotation 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  set_tests_properties( 
    test_video_annotation PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
endif()
