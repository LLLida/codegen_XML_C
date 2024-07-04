cmake_minimum_required(VERSION 3.10)
project(test_nwogen VERSION 0.1)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(nwogen)

add_executable(codegen "main.cpp")

target_link_libraries(codegen nwogen)
