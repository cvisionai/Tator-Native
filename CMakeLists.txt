cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( fish_annotator )

set( CMAKE_BUILD_TYPE Release )

option( BUILD_TESTS           "Whether to build unit tests."            OFF )
option( BUILD_DOCS            "Whether to enable documentation target." ON  )
option( BUILD_VIDEO_ANNOTATOR "Whether to build video annotator."       ON  )
option( BUILD_IMAGE_ANNOTATOR "Whether to build image annotator."       ON  )
option( BUILD_DB_UPLOADER     "Whether to build database uploader."     OFF )

if( MSVC )
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /MP" )
elseif( APPLE )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
elseif( UNIX )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -Wl,--no-as-needed" )
endif()

# Set the default install prefix
if( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  set( 
    CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/inst" 
    CACHE PATH "Default install path" FORCE 
    )
endif()

# Set the cmake module path
set( CMAKE_MODULE_PATH ${FISHDET_CMAKE_DIR} )

# Do Qt specific settings
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC ON )

# Generate the library hints file if necessary
set( HINTS_PATH "${FISHDET_CMAKE_DIR}/FishDetFindLibsHints.cmake" )
if( EXISTS "${HINTS_PATH}" )
  message( STATUS "Found library hints file at ${HINTS_PATH}" )
else()
  message( STATUS "Generating library hints file at ${HINTS_PATH}" )
  message( STATUS "This file can be modified to work with your build system" )
  configure_file( "${HINTS_PATH}.in" "${HINTS_PATH}" )
endif()

# Find necessary third party libraries
include( "cmake/FishDetFindLibsHints.cmake" )
include( "cmake/FishDetFindLibs.cmake" )

# Add necessary include directories
include_directories(
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${FISHDET_INCLUDE_DIR}
  )

enable_testing()

if( WIN32 )
  set( FISHDET_ICON "res/icons/cvision\\\\cvision_no_text.ico" )
  set( FISHDET_BMP "res/icons/cvision\\\\cvision_no_text.bmp" )
elseif( APPLE )
  set( FISHDET_ICON "cvision_no_text.icns" )
  set( FISHDET_ICON_FULL "res/icons/cvision/cvision_no_text.icns" )
endif()
set( FISHDET_RES_FILE "res/resources.qrc" )

add_subdirectory( "src" )
if( ${BUILD_TESTS} )
  add_subdirectory( "test" )
endif()
if( ${BUILD_DOCS} )
  add_subdirectory( "doc" )
endif()

# Configure CPack
set( CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "." )
include( InstallRequiredSystemLibraries )
set( CPACK_PACKAGE_NAME "FishAnnotator" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY 
     "An application for assisting estimation of fish populations." )
set( CPACK_PACKAGE_VENDOR "CVision Consulting LLC" )
set( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md" )
set( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md" )
set( CPACK_PACKAGE_VERSION "1.0.0" )
set( CPACK_PACKAGE_VERSION_MAJOR "1" )
set( CPACK_PACKAGE_VERSION_MINOR "0" )
set( CPACK_PACKAGE_VERSION_PATCH "0" )
set( CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME} )
if( WIN32 )
  set( CPACK_GENERATOR "NSIS" )
  set( CPACK_NSIS_MUI_ICON ${FISHDET_ICON} )
  set( CPACK_NSIS_MUI_HEADERIMAGE_BITMAP ${FISHDET_ICON} )
  set( CPACK_NSIS_MUI_HEADERIMAGE_RIGHT ON )
  set( CPACK_NSIS_MUI_UNIICON ${FISHDET_ICON} )
  set( CPACK_NSIS_MUI_UNIHEADERIMAGE_BITMAP ${FISHDET_ICON} )
  set( CPACK_NSIS_MUI_UNIHEADERIMAGE_RIGHT ON )
  set( CPACK_NSIS_INSTALLED_ICON_NAME ${FISHDET_ICON} )
  set( CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME} )
  set( CPACK_NSIS_CONTACT "benjamin.woodward@cvisionconsulting.com" )
  set( CPACK_NSIS_MODIFY_PATH ON )
  set( CPACK_NSIS_EXECUTABLES_DIRECTORY "." )
  set( CPACK_CREATE_DESKTOP_LINKS video_annotator video_slicer image_annotator Uninstall)
elseif( APPLE )
  set( CPACK_GENERATOR "DragNDrop" )
elseif( UNIX )
  set( CPACK_GENERATOR "STGZ" )
  set( CPACK_SOURCE_STRIP_FILES "" )
else()
  message( FATAL_ERROR "Unsupported operating system." )
endif()
set( CPACK_PACKAGE_EXECUTABLES 
  video_annotator "Video Annotator" 
  video_slicer "Video Slicer" 
  image_annotator "Image Annotator" 
  uninstall "Uninstall" 
  )
include( CPack )

