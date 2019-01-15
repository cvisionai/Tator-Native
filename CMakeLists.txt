cmake_minimum_required( VERSION 3.5.1 FATAL_ERROR )

project( tator )

set( CMAKE_BUILD_TYPE Release )

option( BUILD_DOCS            "Whether to enable documentation target." ON  )
option( BUILD_VIDEO_ANNOTATOR "Whether to build video annotator."       ON  )
option( BUILD_IMAGE_ANNOTATOR "Whether to build image annotator."       ON  )
option( BUILD_DB_UPLOADER     "Whether to build database uploader."     OFF )
option( BUILD_INSTALLER       "Whether to build cpack target."          OFF )

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

# Do Qt specific settings
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC ON )

# Find necessary third party libraries
include( "cmake/TatorFindLibs.cmake" )

# Add necessary include directories
include_directories(
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

if( WIN32 )
  set( TATOR_ICON 
    "${CMAKE_CURRENT_SOURCE_DIR}/res/icons/cvision\\\\cvision_no_text.ico" )
  set( TATOR_BMP 
    "${CMAKE_CURRENT_SOURCE_DIR}/res/icons/cvision\\\\cvision_no_text.bmp" )
elseif( APPLE )
  set( TATOR_ICON "cvision_no_text.icns" )
  set( TATOR_ICON_FULL 
    "${CMAKE_CURRENT_SOURCE_DIR}/res/icons/cvision/cvision_no_text.icns" )
endif()
set( TATOR_RES_FILE 
  "${CMAKE_CURRENT_SOURCE_DIR}/res/resources.qrc" )

add_subdirectory( "src" )
add_subdirectory( "config" )
if( ${BUILD_DOCS} )
  add_subdirectory( "doc" )
endif()

# Configure CPack
if( ${BUILD_INSTALLER} )
  set( CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "." )
  include( InstallRequiredSystemLibraries )
  set( CPACK_PACKAGE_NAME "Tator" )
  set( CPACK_PACKAGE_DESCRIPTION_SUMMARY 
       "An application for video and image annotation." )
  set( CPACK_PACKAGE_VENDOR "CVision AI Inc" )
  set( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md" )
  set( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md" )
  set( CPACK_PACKAGE_VERSION "0.4.0" )
  set( CPACK_PACKAGE_VERSION_MAJOR "0" )
  set( CPACK_PACKAGE_VERSION_MINOR "4" )
  set( CPACK_PACKAGE_VERSION_PATCH "0" )
  set( CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME} )
  if( WIN32 )
    set( CPACK_GENERATOR "NSIS" )
    set( CPACK_NSIS_MUI_ICON ${TATOR_ICON} )
    set( CPACK_NSIS_MUI_HEADERIMAGE_BITMAP ${TATOR_ICON} )
    set( CPACK_NSIS_MUI_HEADERIMAGE_RIGHT ON )
    set( CPACK_NSIS_MUI_UNIICON ${TATOR_ICON} )
    set( CPACK_NSIS_MUI_UNIHEADERIMAGE_BITMAP ${TATOR_ICON} )
    set( CPACK_NSIS_MUI_UNIHEADERIMAGE_RIGHT ON )
    set( CPACK_NSIS_INSTALLED_ICON_NAME ${TATOR_ICON} )
    set( CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME} )
    set( CPACK_NSIS_CONTACT "info@cvisionai.com" )
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
endif()

