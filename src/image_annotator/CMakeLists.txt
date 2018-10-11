# Find the sources for image annotator
file( GLOB IMAGE_ANNOTATOR_SOURCES "*.cc" )

include_directories(
  "${CMAKE_CURRENT_BINARY_DIR}/../common/common_autogen/include"
  "${CMAKE_CURRENT_BINARY_DIR}/../common/common_autogen/include_${CMAKE_BUILD_TYPE}"
  "${CMAKE_CURRENT_BINARY_DIR}/../common"
  "../common"
)

# Add image_annotator executable
if( WIN32 )
  file( COPY ${TATOR_ICON} DESTINATION . )
  file( GLOB IMAGE_ANNOTATOR_RESOURCES "*.rc" )
  add_executable( image_annotator WIN32 
    ${IMAGE_ANNOTATOR_SOURCES} 
    ${IMAGE_ANNOTATOR_RESOURCES} 
    ${TATOR_RES_FILE} 
    )
  target_link_libraries(
    image_annotator
    common
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    Qt5::Gui
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
  set_target_properties( 
    image_annotator 
    PROPERTIES 
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS" 
    )
elseif( APPLE )
  add_executable( image_annotator MACOSX_BUNDLE
    ${IMAGE_ANNOTATOR_SOURCES} 
    ${TATOR_ICON_FULL} 
    ${TATOR_RES_FILE} 
    )
  set_target_properties( image_annotator PROPERTIES
    MACOSX_BUNDLE_ICON_FILE ${TATOR_ICON}
    OUTPUT_NAME "Image Annotator" 
  )
  set_source_files_properties( ${TATOR_ICON_FULL} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources" )
  target_link_libraries(
    image_annotator
    common
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::PrintSupport
    ${QT_THIRD_PARTY_LIBS}
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
elseif( UNIX )
  add_executable( image_annotator
    ${IMAGE_ANNOTATOR_SOURCES}
    ${TATOR_RES_FILE} 
    )
  target_link_libraries(
    image_annotator
    dl
    common
    ${UNIX_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::PrintSupport
    ${QT_THIRD_PARTY_LIBS}
    Qt5::DBus
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
    )
endif()

# Add install target 
install( 
  TARGETS image_annotator 
  DESTINATION . 
  )

