project(function_traits_test)
message(NOTICE "function traits")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
