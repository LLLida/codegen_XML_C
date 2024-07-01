cmake_minimum_required(VERSION 3.10)
project(test_nwogen VERSION 0.1)

add_subdirectory(nwogen)

add_executable(test_nwogen "main.cpp")

target_link_libraries(test_nwogen nwogen)
