cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(Kraken LANGUAGES CXX)
include_directories(source/library/core)
include_directories(source/library)
add_executable(${PROJECT_NAME}
  tests/matrix_tests.cpp
  tests/numeric_tests.cpp
  tests/numeric_methods_tests.cpp
)
