add_library(
        swift_compiler
        STATIC
        SourceManager.cpp
        DiagnosticEngine.cpp
        Lexer.cpp
        CharInfo.cpp
)

target_include_directories(swift_compiler PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${LLVM_INCLUDE_DIRS}
)

# Map LLVM components to libraries and link with the compiler library only
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(swift_compiler PRIVATE ${llvm_libs})

# Executable target
add_executable(swiftc
        main.cpp
)

target_include_directories(swiftc PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${LLVM_INCLUDE_DIRS}
)

# Just link the compiler library; it already pulls in LLVM libs
target_link_libraries(swiftc PRIVATE swift_compiler)
