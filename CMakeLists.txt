cmake_minimum_required(VERSION 3.20)
project(CompileTimeGraph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Library (header-only)
add_library(ctgraph INTERFACE)
target_include_directories(ctgraph INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Examples
add_executable(example_petersen examples/example_petersen.cpp)
target_link_libraries(example_petersen PRIVATE ctgraph)

add_executable(example_complete examples/example_complete.cpp)
target_link_libraries(example_complete PRIVATE ctgraph)

# Tests (optional)
enable_testing()
add_subdirectory(tests)
