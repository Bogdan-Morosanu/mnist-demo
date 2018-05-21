cmake_minimum_required (VERSION 2.6)

project (demo)

add_executable(demo demo.cpp)

add_subdirectory(app)
add_subdirectory(threads)
add_subdirectory(parser)
add_subdirectory(point-cloud)

include_directories("${PROJECT_SOURCE_DIR}" "/usr/include/eigen3")

target_link_libraries(demo app threads parser ptc boost_program_options pthread)
