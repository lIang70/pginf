CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

PROJECT(pginf VERSION 0.1)

MESSAGE( STATUS "CMAKE_C_COMPILER_ID = ${CMAKE_C_COMPILER_ID}" )
MESSAGE( STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}" )

MESSAGE( STATUS "CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}" )
MESSAGE( STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}" )
MESSAGE( STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}" )

MESSAGE( STATUS "CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM} ")

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 11)

OPTION(pginf_build_tests "Build all of pginf's own tests." OFF)
OPTION(pginf_build_with_debuginfo "Build pginf with DEBUG-INFO" OFF)

IF (pginf_build_with_debuginfo)
    SET(CMAKE_BUILD_TYPE RelWithDebInfo)
    IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ggdb -pthread")
    ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /DEBUG /Zi /EHa")
    ENDIF()
ELSE()
    SET(CMAKE_BUILD_TYPE MinSizeRel)
    IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread")
    ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /EHa")
    ENDIF()
ENDIF()
 
# add 3rd lib
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# add sub-project
ADD_SUBDIRECTORY(pginf)

# add googletest
if (pginf_build_tests)

    INCLUDE(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )

    IF(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
 
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
 
    ENDIF()

    # To enable test
    ENABLE_TESTING()
    INCLUDE(GoogleTest)

endif()
