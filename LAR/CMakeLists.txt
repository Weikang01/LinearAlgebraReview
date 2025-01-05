cmake_minimum_required(VERSION 3.10)
project(LAR VERSION 0.0.1 LANGUAGES CXX)

file(GLOB_RECURSE _srcs src/*.cpp)

add_library(LAR SHARED ${_srcs})

set_target_properties(LAR PROPERTIES
  CXX_STANDARD 17
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN 1
)

target_include_directories(LAR
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include # Headers in source directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src # Source files in source directory
)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)