cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(pginf_tests)

include_directories(${CMAKE_SOURCE_DIR}/pginf/src ${CMAKE_SOURCE_DIR}/include/)

file(GLOB ALL_TARGETS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

foreach(TARGET IN LISTS ALL_TARGETS)
    # Get file name without directory
    # get_filename_component(TARGET_DIR ${TARGET} DIRECTORY)
    get_filename_component(TARGET_NAME ${TARGET} NAME_WE)
    add_executable(${TARGET_NAME} ${TARGET})
    target_link_libraries(${TARGET_NAME} pginf_base GTest::gtest_main)
    message( STATUS "Add test target[${TARGET_NAME}](${TARGET})")
    # add_test(NAME ${TARGET_NAME} COMMAND "${TARGET_NAME}" --build ${CMAKE_BINARY_DIR} --target ${TARGET_NAME})
    gtest_discover_tests(${TARGET_NAME})

endforeach()