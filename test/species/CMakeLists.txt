set( TEST_SPECIES_SOURCES test_species.cc )
set( SPECIES_SOURCES
  "../../src/common/serialization.cc"
  "../../src/common/species.cc"
  )
include_directories( 
  "../../include/fish_detector/common"
  )
if( WIN32 )
  add_executable( test_species WIN32 
    ${TEST_SPECIES_SOURCES} 
    ${SPECIES_SOURCES}
    )
  target_link_libraries( 
    test_species 
    ${WINDOWS_LIBRARIES}
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    )
  set_target_properties(
    test_species
    PROPERTIES
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:CONSOLE"
    )
  add_test( 
    NAME test_species 
    COMMAND test_species 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Release"
    )
  set_tests_properties( 
    test_species PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
endif()
