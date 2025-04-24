#include <iostream>
#include <fstream>
#include <string>

#include "swift/Lexer/Lexer.h"
#include "swift/Source/SourceManager.h"
#include "swift/Diagnostic/DiagnosticEngine.h"

// Custom diagnostic consumer to capture diagnostics
class MyDiagnosticConsumer : public swift::DiagnosticConsumer {
public:
    bool hadError = false;
    
    void handleDiagnostic(const swift::Diagnostic &Diag, const swift::SourceManager &SM) override {
        std::string severity;
        switch (Diag.Severity) {
            case swift::DiagnosticSeverity::Error:
                severity = "error";
                hadError = true;
                break;
            case swift::DiagnosticSeverity::Warning:
                severity = "warning";
                break;
            case swift::DiagnosticSeverity::Note:
                severity = "note";
                break;
            case swift::DiagnosticSeverity::Remark:
                severity = "remark";
                break;
        }
        
        std::cout << "Diagnostic [" << severity << "]: " << Diag.Message << std::endl;
        
        // Only try to get location info if location is valid
        if (Diag.Location.isValid()) {
            try {
                unsigned BufferID = SM.findBufferContainingLoc(Diag.Location);
                if (BufferID != ~0U) {
                    unsigned Offset = SM.getLocOffsetInBuffer(Diag.Location, BufferID);
                    
                    // Calculate line and column from offset
                    llvm::StringRef BufferStr = SM.getBufferContent(BufferID);
                    unsigned Line = 1;
                    unsigned LastLineStart = 0;
                    
                    for (unsigned i = 0; i < Offset && i < BufferStr.size(); ++i) {
                        if (BufferStr[i] == '\n') {
                            ++Line;
                            LastLineStart = i + 1;
                        }
                    }
                    
                    unsigned Column = Offset - LastLineStart + 1;
                    std::cout << "  at line " << Line << ", column " << Column << std::endl;
                }
            } catch (...) {
                std::cout << "  (location information unavailable)" << std::endl;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <swift-file-path>" << std::endl;
        return 1;
    }

    // Get file path from command line
    std::string filePath = argv[1];

    std::cout << "-------------------" << std::endl;
    std::cout << "Lexical Analysis of Swift File: " << filePath << std::endl;
    std::cout << "-------------------" << std::endl;

    // Set up source manager
    swift::SourceManager sourceMgr;
    
    // Set up diagnostic engine with custom consumer
    auto diagConsumer = std::make_unique<MyDiagnosticConsumer>();
    MyDiagnosticConsumer* diagConsumerPtr = diagConsumer.get(); // Keep a pointer for later use
    swift::DiagnosticEngine diagEngine(sourceMgr);
    diagEngine.addConsumer(std::move(diagConsumer));

    // Load file into buffer
    auto fileBufferOrError = llvm::MemoryBuffer::getFile(filePath);
    if (std::error_code EC = fileBufferOrError.getError()) {
        std::cerr << "Error opening file: " << EC.message() << std::endl;
        return 1;
    }

    // Add buffer to source manager
    unsigned bufferID = sourceMgr.addNewSourceBuffer(std::move(fileBufferOrError.get()));

    // Create the lexer with default options
    swift::LangOptions langOpts;
    swift::Lexer lexer(langOpts, sourceMgr, bufferID, &diagEngine,
                      swift::LexerMode::Swift,
                      swift::HashbangMode::Allowed,
                      swift::CommentRetentionMode::ReturnAsTokens);

    // Tokenize the input
    int tokenCount = 0;
    swift::Token token;
    std::vector<swift::Token> tokens;

    try {
        do {
            lexer.lex(token);
            
            // Store token if it's valid
            if (token.getKind() != swift::tok::NUM_TOKENS) {
                tokens.push_back(token);
                tokenCount++;
            }
        } while (token.isNot(swift::tok::eof));
    } catch (const std::exception& e) {
        std::cerr << "Exception during lexing: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception during lexing" << std::endl;
        return 1;
    }

    // Report results
    if (diagConsumerPtr->hadError) {
        std::cout << "-------------------" << std::endl;
        std::cout << "Lexing failed with errors" << std::endl;
        std::cout << "-------------------" << std::endl;
        return 1;
    } else {
        std::cout << "-------------------" << std::endl;
        std::cout << "Lexing successful! Found " << tokenCount << " tokens:" << std::endl;
        std::cout << "-------------------" << std::endl;
        
        // Only print first 50 tokens to avoid overwhelming output
        const size_t MAX_TOKENS_TO_PRINT = 50;
        const size_t tokensToPrint = std::min(tokens.size(), MAX_TOKENS_TO_PRINT);
        
        // Print tokens
        for (size_t i = 0; i < tokensToPrint; i++) {
            const auto& tok = tokens[i];
            
            std::cout << "Token " << i+1 << ": Kind=" << static_cast<int>(tok.getKind());
            
            // Print token text if possible, but check text size first
            llvm::StringRef text = tok.getText();
            if (!text.empty()) {
                // Limit the token text to first 40 chars if it's very long
                const size_t MAX_TEXT_LEN = 40;
                std::string tokenText = text.substr(0, MAX_TEXT_LEN).str();
                if (text.size() > MAX_TEXT_LEN) {
                    tokenText += "...";
                }
                // Replace newlines with \n for display
                size_t pos = 0;
                while ((pos = tokenText.find('\n', pos)) != std::string::npos) {
                    tokenText.replace(pos, 1, "\\n");
                    pos += 2;
                }
                std::cout << ", Text=\"" << tokenText << "\"";
            }
            
            std::cout << std::endl;
        }
        
        if (tokens.size() > MAX_TOKENS_TO_PRINT) {
            std::cout << "... and " << (tokens.size() - MAX_TOKENS_TO_PRINT) << " more tokens" << std::endl;
        }
    }

    std::cout << "-------------------" << std::endl;
    return 0;
}