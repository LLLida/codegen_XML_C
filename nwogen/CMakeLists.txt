cmake_minimum_required(VERSION 3.10)
project(nwogen VERSION 0.1)

add_library(nwogen STATIC
  "backend.hpp"
  "backend_c.cpp"
  "backend_c.hpp"
  "block.cpp"
  "block.hpp"
  "codegen.hpp"
  "codegen.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
