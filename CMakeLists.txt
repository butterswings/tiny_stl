cmake_minimum_required(VERSION 3.28)

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TOOLCHAIN_FILE "D:/ScoopApp/Scoop/apps/vcpkg/current/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic)

project(tiny_stl)

find_path(
  DBG_MACRO_INCLUDE_DIRS
  NAMES dbg.h
  HINTS D:/ScoopApp/Scoop/apps/vcpkg/current/packages/dbg-macro_x64-mingw-dynamic/include
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${DBG_MACRO_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

option(BUILD_TEST "Whether or not to build the tests" ON)
if (${BUILD_TEST})
    message(STATUS "Building tests")
    enable_testing()
    add_subdirectory(test/meta/function)
endif ()
