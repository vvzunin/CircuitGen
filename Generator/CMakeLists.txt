cmake_minimum_required(VERSION 3.14)

project(CircuitGen LANGUAGES CXX)

set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17 -fPIC")
SET(GCC_COVERAGE_LINK_FLAGS "-lgcov")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
include_directories(lib)

include(ExternalProject)
include(FetchContent)

option(CircuitGenGenerator_BUILD_TESTS "Build CircuitGen tests" ON)

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

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
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

target_include_directories(
  CircuitGen ${warning_guard}
  PUBLIC
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
)

target_link_libraries(CircuitGen PRIVATE
 easylogging++)

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_TESTS "Build test." ON)

  if(BUILD_TESTS)
    message("Add tests")
    add_subdirectory(test)
  endif()
endif()

# ---- Developer mode ----
# if(NOT CircuitGenGenerator_DEVELOPER_MODE)
#   return()
# elseif(NOT PROJECT_IS_TOP_LEVEL)
#   message(
#     AUTHOR_WARNING
#     "Developer mode is intended for developers of CircuitGenGenerator"
#   )
# endif()

# include(cmake/dev-mode.cmake)