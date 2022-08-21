
# test func
FUNCTION(CXX_TEST name output)

    ADD_EXECUTABLE(${name} "test/${name}.cpp" ${ARGN})
    SET_TARGET_PROPERTIES(${name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${output}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${output}
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${output}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${output})
    IF(MSVC)
        # BigObj required for tests.
        SET(CXX_FLAGS "${CXX_FLAGS} -bigobj")
    ENDIF()
    ADD_TEST(NAME ${name} COMMAND "$<TARGET_FILE:${name}>")

ENDFUNCTION(CXX_TEST)

# googletest func
FUNCTION(GOOGLE_CXX_TEST_UNIT name output)

    ADD_EXECUTABLE(${name} "test/${name}.cc")
    TARGET_LINK_LIBRARIES(${name} GTest::gtest_main)
    SET_TARGET_PROPERTIES(${name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${output}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${output}
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${output}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${output})
    IF(MSVC)
        # BigObj required for tests.
        SET(CXX_FLAGS "${CXX_FLAGS} -bigobj")
    ENDIF()
    gtest_discover_tests(${name})

ENDFUNCTION(GOOGLE_CXX_TEST_UNIT)

# googletest func
FUNCTION(GOOGLE_CXX_TEST_DLL name output)

    ADD_LIBRARY(${name} SHARED "test/${name}.cc")
    TARGET_LINK_LIBRARIES(${name} PRIVATE GTest::gtest_main ${ARGN})
    SET_TARGET_PROPERTIES(${name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${output}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${output}
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${output}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${output})
    IF(MSVC)
        # BigObj required for tests.
        SET(CXX_FLAGS "${CXX_FLAGS} -bigobj")
    ENDIF()

ENDFUNCTION(GOOGLE_CXX_TEST_DLL)

# googletest func
FUNCTION(GOOGLE_CXX_TEST_INTEGRA name output)

    ADD_EXECUTABLE(${name} "test/${name}.cc")
    TARGET_LINK_LIBRARIES(${name} PRIVATE GTest::gtest_main ${ARGN})
    SET_TARGET_PROPERTIES(${name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${output}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${output}
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${output}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${output})
    IF(MSVC)
        # BigObj required for tests.
        SET(CXX_FLAGS "${CXX_FLAGS} -bigobj")
    ENDIF()
    gtest_discover_tests(${name})

ENDFUNCTION(GOOGLE_CXX_TEST_INTEGRA)

# Generate PginF plugin xml
FUNCTION(GENERATE_PGINF_XML name type version uninstall_need_reload outputpath)

    FILE(REMOVE ${outputpath})
    FILE(GENERATE 
        OUTPUT ${outputpath}
        CONTENT
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<plugin type=${type} id=${name}.${version}>
    <!-- ... define some plugin parameters-->
    <name>${name}</name>
    <version>${version}</version>
    <uninstall_need_reload>${uninstall_need_reload}</uninstall_need_reload>
    <need_load>true</need_load>
</plugin>
"
    )

ENDFUNCTION(GENERATE_PGINF_XML)