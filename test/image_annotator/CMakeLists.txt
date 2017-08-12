set( TEST_IMAGE_ANNOTATOR_SOURCES test_image_annotator.cc )
set( IMAGE_ANNOTATOR_SOURCES
  "../../src/image_annotator/image_annotation.cc"
  "../../src/image_annotator/mainwindow.cc"
  )
include_directories(
  "../../src/image_annotator"
  "../../src/common"
  "${CMAKE_CURRENT_BINARY_DIR}/../../src/common/common_autogen/include"
  "${CMAKE_CURRENT_BINARY_DIR}/../../src/common"
  )
if( WIN32 )
  add_executable( test_image_annotator WIN32
    ${TEST_IMAGE_ANNOTATOR_SOURCES}
    ${IMAGE_ANNOTATOR_SOURCES}
    )
  target_link_libraries(
    test_image_annotator
    common
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
  file( COPY crashed DESTINATION Release )
  file( COPY blah.json DESTINATION Release )
elseif( APPLE )
  add_executable( test_image_annotator
    ${TEST_IMAGE_ANNOTATOR_SOURCES}
    ${IMAGE_ANNOTATOR_SOURCES}
    )
  target_link_libraries(
    test_image_annotator
    common
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
  add_test(
    NAME test_image_annotator
    COMMAND test_image_annotator
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  set_tests_properties(
    test_image_annotator PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
  file( COPY crashed DESTINATION . )
  file( COPY blah.json DESTINATION . )
elseif( UNIX )
  add_executable( test_image_annotator
    ${TEST_IMAGE_ANNOTATOR_SOURCES}
    ${IMAGE_ANNOTATOR_SOURCES}
    )
  target_link_libraries(
    test_image_annotator
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
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
  add_test(
    NAME test_image_annotator
    COMMAND test_image_annotator
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  set_tests_properties(
    test_image_annotator PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
  file( COPY crashed DESTINATION . )
  file( COPY blah.json DESTINATION . )
endif()
