cmake_minimum_required(VERSION 3.10)
project(LAR VERSION 0.0.1 LANGUAGES CXX)

add_library(LAR INTERFACE)

set_target_properties(LAR PROPERTIES
  CXX_STANDARD 11
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN 1
)

include(GenerateExportHeader)
generate_export_header(LAR EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/LAR_export.h)

target_include_directories(LAR
  INTERFACE
    ${CMAKE_CURRENT_BINARY_DIR}/include # Generated export header
    ${CMAKE_CURRENT_SOURCE_DIR}/include # Headers in source directory
)
