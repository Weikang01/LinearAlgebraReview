cmake_minimum_required(VERSION 3.0)

project(LinearAlgebraReview VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(LAR)

include_directories(PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/LAR/include ${CMAKE_CURRENT_BINARY_DIR}/LAR/include)

add_executable(${PROJECT_NAME} main.cpp)