add_executable(plugin_test main.cc)
target_link_libraries(plugin_test pginf_base pginf_core GTest::gtest_main)
set_target_properties(plugin_test   PROPERTIES
                                    RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin/test/
                                    RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin/test/
                                    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin/test/
                                    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin/test/)
gtest_discover_tests(plugin_test)
message( STATUS "Add test target[plugin_test](integration test)")