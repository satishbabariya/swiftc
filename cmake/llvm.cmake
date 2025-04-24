# Set LLVM directory
set(LLVM_DIR "/Users/satishbabariya/Downloads/LLVM-20.1.3-macOS-ARM64/lib/cmake/llvm")

set(CMAKE_C_COMPILER "/Users/satishbabariya/Downloads/LLVM-20.1.3-macOS-ARM64/bin/clang")
set(CMAKE_CXX_COMPILER "/Users/satishbabariya/Downloads/LLVM-20.1.3-macOS-ARM64/bin/clang++")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add LLVM include directories and definitions
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})