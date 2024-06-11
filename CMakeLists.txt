cmake_minimum_required(VERSION 3.22)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(tiny_stl)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

option(BUILD_TEST "Whether or not to build the tests" ON)
if (${BUILD_TEST})
  message(STATUS "Building tests")
  enable_testing()
  add_subdirectory(test/meta/function)
endif ()
