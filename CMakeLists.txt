cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(pginf VERSION 0.1 LANGUAGES CXX)

message( STATUS "CMAKE_C_COMPILER_ID = ${CMAKE_C_COMPILER_ID}" )
message( STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}" )

message( STATUS "CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}" )
message( STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}" )
message( STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}" )

message( STATUS "CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM} ")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

if(NOT PGINF__LIBRARY_TYPE)
    set(PGINF__LIBRARY_TYPE SHARED CACHE STRING "Set library type to SHARED/STATIC")
endif()
message( STATUS "PGINF__LIBRARY_TYPE: ${PGINF__LIBRARY_TYPE} ")

if(NOT BUILD_TESTS)
    option(BUILD_TESTS "Build all of own tests." OFF)
endif()

if(NOT BUILD_WITH_DEBUGINFO)
    option(BUILD_WITH_DEBUGINFO "Build with DEBUG-INFO" OFF)
endif()

if(BUILD_WITH_DEBUGINFO)
    add_definitions(-DPGINF_DEBUG)    
    set(CMAKE_BUILD_TYPE Debug)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb -pthread")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-recover=address -fno-stack-protector -fno-omit-frame-pointer")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O0 /DEBUG /Zi /EHa")
    endif()
else()
    set(CMAKE_BUILD_TYPE MinSizeRel)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /EHa")
    endif()
endif()

message( STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE} ")

add_subdirectory(pginf)