cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(
-Wsign-conversion
-Wconversion
-Wcast-align
-Wnon-virtual-dtor
-Wshadow
-Wpedantic
-Wall
-Wextra)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(Kraken LANGUAGES CXX)

add_executable(${PROJECT_NAME}
  tests/matrix_tests.cpp
  tests/numeric_tests.cpp
  tests/numeric_methods_tests.cpp
)
