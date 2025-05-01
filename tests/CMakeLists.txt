# Executable target
add_executable(swift-lexer-tests
        lexer_tests.cpp
)

target_include_directories(swift-lexer-tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${LLVM_INCLUDE_DIRS}
        ${GTEST_INCLUDE_DIRS}
)

# Just link the compiler library; it already pulls in LLVM libs
target_link_libraries(swift-lexer-tests PRIVATE swift_compiler GTest::gtest GTest::gtest_main)