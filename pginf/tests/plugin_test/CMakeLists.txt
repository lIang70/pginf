add_library(decode SHARED decode.cc)
target_link_libraries(decode pginf_base pginf_core GTest::gtest_main)
set_target_properties(decode    PROPERTIES
                                LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin/test/plugin/
                                LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin/test/plugin/
                                LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin/test/plugin/
                                LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin/test/plugin/)

add_library(net SHARED net.cc)
target_link_libraries(net pginf_base pginf_core GTest::gtest_main)
set_target_properties(net   PROPERTIES
                            LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin/test/plugin/
                            LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin/test/plugin/
                            LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin/test/plugin/
                            LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin/test/plugin/)

add_executable(plugin_test main.cc)
target_link_libraries(plugin_test pginf_base pginf_core GTest::gtest_main)
set_target_properties(plugin_test   PROPERTIES
                                    RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin/test/
                                    RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin/test/
                                    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin/test/
                                    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin/test/)
gtest_discover_tests(plugin_test)
message( STATUS "Add test target[plugin_test](integration test)")