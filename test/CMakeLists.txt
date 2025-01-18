cmake_minimum_required(VERSION 3.14)

project(CircuitGenTest LANGUAGES CXX)

 set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17 -fPIC")
 SET(GCC_COVERAGE_LINK_FLAGS "-lgcov")
 set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)
include(FetchContent)
include(CTest)

FetchContent_Declare(
  gtest

  # Url for googletest download.
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(gtest)

FetchContent_Declare(
  CircuitGen_Generator
  GIT_REPOSITORY https://github.com/vvzunin/CircuitGen_Generator
  GIT_TAG 53310b2f4
)
FetchContent_MakeAvailable(CircuitGen_Generator)

 FetchContent_Declare(
   OptimizationsVerilogLib
   GIT_REPOSITORY https://github.com/vvzunin/CircuitGen_Parameters
   GIT_TAG 0.5.1
 )
 FetchContent_MakeAvailable(OptimizationsVerilogLib)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

enable_testing()

# ---- Tests ----

configure_file(
  ../Generator/easylogging++.conf
  easylogging++.conf
  COPYONLY
)

function(add_new_test name)
  add_executable(
    CircuitGen_test_${name}
    src/${name}.cpp
  )

  target_link_libraries(
    CircuitGen_test_${name}
    PRIVATE
      GTest::gtest_main
      ${EXTERNAL_LIBS}
      CircuitGenGenerator::CircuitGenGenerator
      CircuitGenGraph::CircuitGenGraph
      OptimizationsVerilogLib::OptimizationsVerilogLib
    
  )

  target_compile_features(CircuitGen_test_${name} PRIVATE cxx_std_17)

  target_include_directories(
    CircuitGen_test_${name} ${warning_guard}
    PUBLIC
      "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../Generator>"
      "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  )
  add_test(NAME ${name} COMMAND CircuitGen_test_${name})
endfunction()

add_new_test("MainTest")


