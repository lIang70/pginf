# tinyxml's CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0)

PROJECT(tinyxml)

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/internal_utils.cmake)

ADD_DEFINITIONS(-DTIXML_USE_STL)

# add header
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

FILE(GLOB PROJECT_SOURCES "src/*.*pp")

# Add the source code to the project's executable。
ADD_LIBRARY(tinyxml STATIC ${PROJECT_SOURCES})
# Set properties: output path
SET_TARGET_PROPERTIES(tinyxml PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)