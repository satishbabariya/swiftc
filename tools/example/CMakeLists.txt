# Executable target
add_executable(swift-example
        main.cpp
)

target_include_directories(swift-example PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${LLVM_INCLUDE_DIRS}
)

# Just link the compiler library; it already pulls in LLVM libs
target_link_libraries(swift-example PRIVATE swift_compiler)