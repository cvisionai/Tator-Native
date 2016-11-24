set( TEST_IMAGE_ANNOTATOR_SOURCES test_image_annotator.cc )
set( IMAGE_ANNOTATOR_SOURCES
  "../../src/image_annotator/image_annotation.cc"
  "../../src/image_annotator/mainwindow.cc"
  )
include_directories( 
  "../../include/fish_detector/image_annotator"
  "../../include/fish_detector/common"
  )
if( WIN32 )
  add_executable( test_image_annotator WIN32 
    ${TEST_IMAGE_ANNOTATOR_SOURCES} 
    ${IMAGE_ANNOTATOR_SOURCES}
    )
  target_link_libraries( 
    test_image_annotator 
    common
    ${OpenCV_LIBS}
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
  set_target_properties(
    test_image_annotator
    PROPERTIES
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:CONSOLE"
    )
  add_test( 
    NAME test_image_annotator 
    COMMAND test_image_annotator 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Release"
    )
  set_tests_properties( 
    test_image_annotator PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
  file( COPY ${OpenCV_SHARED_LIBS} DESTINATION Release )
  file( COPY crashed DESTINATION Release )
endif()
