# pginf's CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0)

PROJECT(pginf)

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/internal_utils.cmake)

ADD_DEFINITIONS(-DPGINF_EXPORTS -DPGINF_USING -DTIXML_USE_STL -DPGINF_DEBUG)

# add header
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

# add tinyxml
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tinyxml/include)
FILE(GLOB TINYXML_SOURCES "3rdparty/tinyxml/src/*.cpp")

# add src
FILE(GLOB PROJECT_SOURCES "src/*.cc")
FILE(GLOB HEADER_SOURCES ${CMAKE_SOURCE_DIR}/include/pginf/*.h)

# Add the source code to the project's executable。
ADD_LIBRARY(pginf STATIC ${TINYXML_SOURCES} ${PROJECT_SOURCES} ${HEADER_SOURCES})
# Set properties: output path
SET_TARGET_PROPERTIES(pginf PROPERTIES  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
                                        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

OPTION(pginf_build_tests "Build all of pginf's own tests." OFF)

if (pginf_build_tests)

    # To enable test
    ENABLE_TESTING()
    INCLUDE(GoogleTest)

    SET(TARGET_LIBRARIES pginf)

    # Add Unit Test
    GOOGLE_CXX_TEST_UNIT(dir_test ${CMAKE_SOURCE_DIR}/bin/test)
    GOOGLE_CXX_TEST_UNIT(read_write_lock_test ${CMAKE_SOURCE_DIR}/bin/test)

    # Add Unit DLL
    GOOGLE_CXX_TEST_DLL(map ${CMAKE_SOURCE_DIR}/bin/test/plugin/map ${TARGET_LIBRARIES})
    GENERATE_PGINF_XML("map" "interface" "20220815" "true" ${CMAKE_SOURCE_DIR}/bin/test/plugin/map.xml)

    GOOGLE_CXX_TEST_DLL(network ${CMAKE_SOURCE_DIR}/bin/test/plugin/network ${TARGET_LIBRARIES})
    GENERATE_PGINF_XML("network" "interface" "20220821" "true" ${CMAKE_SOURCE_DIR}/bin/test/plugin/network.xml)

    # Add Integration Test
    GOOGLE_CXX_TEST_INTEGRA(plugin_test ${CMAKE_SOURCE_DIR}/bin/test ${TARGET_LIBRARIES})

endif()