cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( fish_detector )

set( CMAKE_BUILD_TYPE Release )

if( UNIX )
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
find_package( Doxygen )
if( DOXYGEN_FOUND )
  add_custom_target( doc
    ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/doc/doxyfile"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating API documentation with Doxygen." VERBATIM
    )
endif()

