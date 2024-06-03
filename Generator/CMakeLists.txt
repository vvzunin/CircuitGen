cmake_minimum_required(VERSION 3.14)

project(CircuitGen LANGUAGES CXX)

set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17 -fPIC")
SET(GCC_COVERAGE_LINK_FLAGS "-lgcov")
set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)
include(FetchContent)

FetchContent_Declare(
  OptimizationsVerilogLib
  GIT_REPOSITORY https://github.com/vvzunin/CircuitGen_Parameters
  GIT_TAG 0.5.1
)
FetchContent_MakeAvailable(OptimizationsVerilogLib)

FetchContent_Declare(
  CircuitGen_Generator
  GIT_REPOSITORY https://github.com/vvzunin/CircuitGen_Generator
  GIT_TAG 921f48d
)
FetchContent_MakeAvailable(CircuitGen_Generator)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
  CircuitGenGenerator::CircuitGenGenerator
  OptimizationsVerilogLib::OptimizationsVerilogLib
)

