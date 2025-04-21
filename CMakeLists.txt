cmake_minimum_required(VERSION 3.16.3)

project(swiftc VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add LLVM include directories and definitions
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Add our include directories
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

add_subdirectory(lib)
# add_subdirectory(tools)
# add_subdirectory(tests)

