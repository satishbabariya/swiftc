# Find GTest package
find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
message(STATUS "Found GTest ${GTEST_VERSION}")