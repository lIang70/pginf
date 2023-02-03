# Add header
include_directories(${CMAKE_SOURCE_DIR}/pginf/src ${CMAKE_SOURCE_DIR}/include)

# Add base's src
file(GLOB_RECURSE PGINF_BASE_SOURCES "src/base/*.cc")

# Add the source code to the project's executable。
add_library(pginf_base ${PGINF__LIBRARY_TYPE} ${PGINF_BASE_SOURCES})

target_link_libraries(pginf_base)


# Add core's src
file(GLOB_RECURSE PGINF_CORE_SOURCES "src/core/*.cc")

# Add the source code to the project's executable。
add_library(pginf_core ${PGINF__LIBRARY_TYPE} ${PGINF_CORE_SOURCES})

target_link_libraries(pginf_core pginf_base)


if(BUILD_TESTS)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)

    if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")

        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        if(MSVC)
            # BigObj required for tests.
            set(CXX_FLAGS "${CXX_FLAGS} -bigobj")
        endif()

    endif()

    # To enable test
    enable_testing()
    include(GoogleTest)

    add_subdirectory(tests)

endif()