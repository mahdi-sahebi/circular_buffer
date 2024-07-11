cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(elb_circular_buffer LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)#20
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

# move into the test cmake
option(BUILD_TESTS "To build the tests" OFF)

if (BUILD_TESTS)
  add_subdirectory(test)
endif()