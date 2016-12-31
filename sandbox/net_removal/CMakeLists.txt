cmake_minimum_required(VERSION 2.8)

project(net_removal)

set(CMAKE_BUILD_TYPE Release)

# Set the default install prefix
if( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  set( 
    CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/inst" 
    CACHE PATH "Default install path" FORCE 
    )
endif()

set(NR_PYTHON_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/python")
set(NR_DATA_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(NR_CMAKE_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(NR_SRC_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(NR_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Set the cmake module path
set(CMAKE_MODULE_PATH ${NR_CMAKE_DIR})

# Generate the library hints file if necessary
set( HINTS_PATH "${NR_CMAKE_DIR}/NRFindLibsHints.cmake" )
if( EXISTS "${HINTS_PATH}" )
  message( STATUS "Found library hints file at ${HINTS_PATH}" )
else()
  message( STATUS "Generating library hints file at ${HINTS_PATH}" )
  message( STATUS "This file can be modified to work with your build system" )
  configure_file( "${HINTS_PATH}.in" "${HINTS_PATH}" )
endif()

# Find necessary third party libraries
include( "${NR_CMAKE_DIR}/NRFindLibsHints.cmake" )
include( "${NR_CMAKE_DIR}/NRFindLibs.cmake" )

include_directories(
  ${ArrayFire_INCLUDE_DIRS}
  ${NR_INCLUDE_DIR}
  )

add_subdirectory(src)
add_subdirectory(test)

enable_testing()

add_test(NAME test_partition COMMAND test/Release/test_partition.exe)
set_tests_properties(test_partition PROPERTIES 
  PASS_REGULAR_EXPRESSION "PASSED")
