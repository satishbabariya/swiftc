#include <gtest/gtest.h>
#include <swift/Lexer/Lexer.h>
#include <swift/Lexer/Token.h>
#include <swift/Source/SourceManager.h>
#include <swift/Diagnostic/DiagnosticEngine.h>
#include "llvm/Support/MemoryBuffer.h"

using namespace swift;

class LexerTest : public ::testing::Test {
public:
    LangOptions LangOpts;
    SourceManager SourceMgr;

    [[nodiscard]] std::vector<Token> tokenizeAndKeepEOF(unsigned BufferID) const {
        Lexer L(LangOpts, SourceMgr, BufferID, /*Diags=*/nullptr,
                LexerMode::Swift);
        std::vector<Token> Tokens;
        do {
            Tokens.emplace_back();
            L.lex(Tokens.back());
        } while (Tokens.back().isNot(tok::eof));
        return Tokens;
    }

    std::vector<Token> checkLex(llvm::StringRef Source,
                                llvm::ArrayRef<tok> ExpectedTokens,
                                bool KeepComments = false,
                                const bool KeepEOF = false) {
        const unsigned BufID = SourceMgr.addMemBufferCopy(llvm::MemoryBuffer::getMemBuffer(Source).get());

        std::vector<Token> tokens;
        if (KeepEOF)
            tokens = tokenizeAndKeepEOF(BufID);
        else
            // FIXME: This is a placeholder for the actual tokenization logic in parser
            // tokens = tokenize(LangOpts, SourceMgr, BufID, 0, 0, /*Diags=*/nullptr, KeepComments);
            exit(1);
        EXPECT_EQ(ExpectedTokens.size(), tokens.size());
        for (unsigned i = 0, e = ExpectedTokens.size(); i != e; ++i) {
            EXPECT_EQ(ExpectedTokens[i], tokens[i].getKind()) << "i = " << i;
        }

        return tokens;
    }

    [[nodiscard]] SourceLocation getLocForEndOfToken(const SourceLocation Loc) const {
        return Lexer::getLocForEndOfToken(SourceMgr, Loc);
    }
};

TEST_F(LexerTest, BasicTokenization) {
    // Create a source buffer
    constexpr llvm::StringRef Source = "let x = 42";

    const std::vector<tok> ExpectedTokens = {
        tok::kw_let,
        tok::identifier,
        tok::equal,
        tok::integer_literal,
        tok::eof
    };

    // Call checkLex on the test fixture instance
    checkLex(Source, ExpectedTokens, false, true);
}

TEST_F(LexerTest, EOFTokenLengthIsZero) {
    const char *Source = "meow";
    std::vector<tok> ExpectedTokens{tok::identifier, tok::eof};
    std::vector<Token> Toks = checkLex(Source, ExpectedTokens,
                                       /*KeepComments=*/true,
                                       /*KeepEOF=*/true);
    EXPECT_EQ(Toks[1].getLength(), 0U);
}

// TEST_F(LexerTest, BrokenStringLiteral1) {
//   llvm::StringRef Source("\"meow\0", 6);
//   std::vector<tok> ExpectedTokens{ tok::unknown, tok::eof };
//   std::vector<Token> Toks = checkLex(Source, ExpectedTokens,
//                                      /*KeepComments=*/true,
//                                      /*KeepEOF=*/true);
//   EXPECT_EQ(Toks[0].getLength(), 6U);
//   EXPECT_EQ(Toks[1].getLength(), 0U);
// }
//
// TEST_F(LexerTest, BrokenStringLiteral2) {
//   llvm::StringRef Source("\"\\(meow\0", 8);
//   std::vector<tok> ExpectedTokens{ tok::unknown, tok::eof };
//   std::vector<Token> Toks = checkLex(Source, ExpectedTokens,
//                                      /*KeepComments=*/true,
//                                      /*KeepEOF=*/true);
//   EXPECT_EQ(Toks[0].getLength(), 8U);
//   EXPECT_EQ(Toks[1].getLength(), 0U);
// }
//
// TEST_F(LexerTest, StringLiteralWithNUL1) {
//   llvm::StringRef Source("\"\0\"", 3);
//   std::vector<tok> ExpectedTokens{ tok::string_literal, tok::eof };
//   std::vector<Token> Toks = checkLex(Source, ExpectedTokens,
//                                      /*KeepComments=*/true,
//                                      /*KeepEOF=*/true);
//   EXPECT_EQ(Toks[0].getLength(), 3U);
//   EXPECT_EQ(Toks[1].getLength(), 0U);
// }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
