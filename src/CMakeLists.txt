cmake_minimum_required (VERSION 2.6)

project (demo)

add_executable(demo main.cpp)

include_directories("${PROJECT_SOURCE_DIR}" "/usr/include/eigen3")

target_link_libraries(demo gtest boost_program_options)
