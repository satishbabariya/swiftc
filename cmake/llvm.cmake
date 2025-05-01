# Set LLVM directory
# Allow LLVM directory to be set from environment or command line, with a default value
if(NOT DEFINED LLVM_DIR)
  # Check environment variable first
  if(DEFINED ENV{LLVM_DIR})
    set(LLVM_DIR $ENV{LLVM_DIR})
  else()
    # Attempt to find LLVM in standard locations
    # This will be tried by find_package below
  endif()
endif()

# Allow compiler override but don't set explicit paths by default
# Compilers will be automatically detected by CMake

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add LLVM include directories and definitions
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})