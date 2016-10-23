cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( fish_detector )

set( CMAKE_BUILD_TYPE Release )

if( UNIX OR APPLE )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
endif()

if( MSVC )
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /MP" )
endif()

# Set the default install prefix
if( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  set( 
    CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/inst" 
    CACHE PATH "Default install path" FORCE 
    )
endif()

# FishDetector subdirectories
set( FISHDET_SRC_DIR       "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set( FISHDET_INCLUDE_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set( FISHDET_CMAKE_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
set( FISHDET_TEST_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/test" )
set( FISHDET_DOC_DIR       "${CMAKE_CURRENT_SOURCE_DIR}/doc" )

# Set the cmake module path
set( CMAKE_MODULE_PATH ${FISHDET_CMAKE_DIR} )

# Do Qt specific settings
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )

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

add_subdirectory( ${FISHDET_SRC_DIR} )

#enable_testing()

#add_test( 
#    NAME "test_all_fish_detector" 
#    COMMAND ${PYTHON_EXECUTABLE} 
#    "${CMAKE_CURRENT_SOURCE_DIR}/test/run_fish_detector_tests.py" 
#    "${CMAKE_INSTALL_PREFIX}"
#    "${CMAKE_CURRENT_BINARY_DIR}"
#    )
#set_tests_properties( 
#    "test_all_fish_detector" PROPERTIES
#    FAIL_REGULAR_EXPRESSION "FAILED"
#    PASS_REGULAR_EXPRESSION "OK"
#    )

# Add documentation target
if( ${DOXYGEN_FOUND} STREQUAL "YES" )
  if( ${LATEX_FOUND} )
    configure_file( ${FISHDET_DOC_DIR}/manual.md.in ${FISHDET_DOC_DIR}/manual.md )
    add_custom_target( doc
      ${DOXYGEN_EXECUTABLE} "${FISH_DET_DOC_DIR}/doxyfile"
      COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/doxygen/latex/make.bat"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      COMMENT "Generating API documentation with Doxygen." VERBATIM
      )
  endif()
endif()

# Set icon paths
set( FISHDET_ICON "${CMAKE_CURRENT_SOURCE_DIR}/doc/icons\\\\FishDetector.ico" )
set( FISHDET_BMP "${CMAKE_CURRENT_SOURCE_DIR}/doc/icons\\\\FishDetector.bmp" )

# Configure CPack
set( CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "." )
include( InstallRequiredSystemLibraries )
set( CPACK_PACKAGE_NAME "FishDetector" )
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
  set( CPACK_NSIS_CONTACT "bgwoodward@gmail.com" )
  set( CPACK_NSIS_MODIFY_PATH ON )
  set( CPACK_NSIS_EXECUTABLES_DIRECTORY "." )
  set( CPACK_CREATE_DESKTOP_LINKS fish_gui video_slicer )
elseif( APPLE )
  set( CPACK_GENERATOR "Bundle" )
  set( CPACK_PACKAGE_FILE_NAME "FishDetector" )
  set( CPACK_PACKAGE_ICON "${FISHDET_ICON}" )
  set( CPACK_BUNDLE_NAME "FishDetector" )
  set( CPACK_BUNDLE_ICON "${FISHDET_ICON}" )
  set( CPACK_BUNDLE_PLIST "" )
  set( CPACK_BUNDLE_STARTUP_COMMAND "" )
elseif( UNIX )
  set( CPACK_GENERATOR "STGZ" )
  set( CPACK_SOURCE_STRIP_FILES "" )
else()
  message( FATAL_ERROR "Unsupported operating system." )
endif()
set( CPACK_PACKAGE_EXECUTABLES 
  fish_gui "Fish GUI" 
  video_slicer "Video Slicer" 
  uninstall "Uninstall" 
  )
include( CPack )

