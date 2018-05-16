cmake_minimum_required (VERSION 2.6)

project (demo)

add_executable(demo demo.cpp)

add_subdirectory(mnet)
add_subdirectory(threads)

include_directories("${PROJECT_SOURCE_DIR}" "/usr/include/eigen3")

target_link_libraries(demo mnet threads gtest boost_program_options pthread)
