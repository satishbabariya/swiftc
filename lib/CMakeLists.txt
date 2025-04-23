add_library(
        swift_compiler
        STATIC

        SourceManager.cpp
        DiagnosticEngine.cpp
        Lexer.cpp
)

target_include_directories(swift_compiler PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${LLVM_INCLUDE_DIRS}
)

#target_link_libraries(swift_lib
#        PRIVATE
#        ${llvm_libs}
#)

# Link against LLVM libraries
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(swift_compiler PRIVATE ${llvm_libs})

# Add the executable target
add_executable(swiftc
        main.cpp
)

target_include_directories(swiftc PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${LLVM_INCLUDE_DIRS}
)

# Link the executable with the swift_compiler library
target_link_libraries(swiftc PRIVATE swift_compiler ${llvm_libs})