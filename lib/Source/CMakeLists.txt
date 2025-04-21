add_library(
    swift_source
    STATIC

    SourceManager.cpp
)

target_include_directories(swift_source PUBLIC
  ${CMAKE_SOURCE_DIR}/include 
  ${LLVM_INCLUDE_DIRS}
)