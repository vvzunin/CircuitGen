project(CircuitGenTestClass LANGUAGES CXX)


cmake_minimum_required(VERSION 3.28)
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
  GIT_TAG 53310b2f4
)
FetchContent_MakeAvailable(CircuitGen_Generator)


add_executable(${PROJECT_NAME} benchmarks.cpp)


target_link_libraries(${PROJECT_NAME}  PRIVATE 
  CircuitGenGenerator::CircuitGenGenerator
  OptimizationsVerilogLib::OptimizationsVerilogLib
)

if(NOT TARGET easylogging++)
  add_library(
    easylogging++ STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/easyloggingpp/easylogging++.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/easyloggingpp/easylogging++.h
  )
endif()

configure_file(
  easylogging++.conf
  easylogging++.conf
  COPYONLY
)

target_link_libraries(${PROJECT_NAME}  PRIVATE easylogging++)

