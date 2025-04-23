#include <iostream>

#include "swift/Lexer/Lexer.h"
#include "swift/Source/SourceManager.h"
#include "swift/Diagnostic/DiagnosticEngine.h"

int main(int argc, char *argv[]) {
    std::string sampleCode = R"(
// This is a sample code
func greet(name: String) -> String {
  let greeting = "Hello, " + name + "!"
  return greeting
}

let result = greet(name: "World")
)";

    std::cout << "-------------------" << std::endl;
    std::cout << "Lexical Analysis of Swift Code:" << std::endl;
    std::cout << "-------------------" << std::endl;

    // Set up source manager and buffer
    swift::SourceManager sourceMgr;
    swift::DiagnosticEngine diagEngine(sourceMgr);

    // Create a buffer from the sample code
    unsigned bufferID = sourceMgr.addNewSourceBuffer(
        llvm::MemoryBuffer::getMemBuffer(sampleCode, "sample.swift"));


    // Create the lexer with default options
    swift::LangOptions langOpts;
    swift::Lexer lexer(langOpts, sourceMgr, bufferID, &diagEngine,
                      swift::LexerMode::Swift,
                      swift::HashbangMode::Allowed,
                      swift::CommentRetentionMode::ReturnAsTokens);

    // Tokenize the input
    swift::Token token;
    do {
        lexer.lex(token);

        // Skip whitespace tokens if desired
        if (token.getKind() != swift::tok::NUM_TOKENS) {
            std::cout << "Token Kind: " << static_cast<int>(token.getKind()) << std::endl;
        }
    } while (token.isNot(swift::tok::eof));

    std::cout << "-------------------" << std::endl;

    return 0;
}