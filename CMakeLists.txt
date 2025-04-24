cmake_minimum_required(VERSION 3.16.3)

project(swiftc VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(fmt)
include(llvm)
include(gtest)

# Add our include directories
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable testing
enable_testing()

add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(tests)