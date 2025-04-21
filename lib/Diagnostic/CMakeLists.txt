add_library(
    swift_diagnostic
    STATIC

    DiagnosticEngine.cpp
)

target_include_directories(swift_diagnostic PUBLIC
  ${CMAKE_SOURCE_DIR}/include 
  ${LLVM_INCLUDE_DIRS}
)