#include <gtest/gtest.h>
#include <swift/Lexer/Lexer.h>
#include <swift/Lexer/Token.h>
#include <swift/Source/SourceManager.h>

using namespace swift;

TEST(LexerTest, BasicTokenization) {
    // Create a source manager and buffer
    SourceManager SM;
    llvm::StringRef Source = "let x = 42";
    unsigned BufferID = SM.addNewSourceBuffer(llvm::MemoryBuffer::getMemBuffer(Source));

    // Create lexer
    Lexer L(LangOptions(), SM, BufferID, nullptr,
             LexerMode::Swift, HashbangMode::Allowed);

    // Test tokens
    Token Tok;

    // First token should be 'let' keyword
    L.lex(Tok);
    EXPECT_EQ(Tok.getKind(), tok::kw_let);
    EXPECT_EQ(Tok.getText(), "let");

    // Second token should be identifier 'x'
    L.lex(Tok);
    EXPECT_EQ(Tok.getKind(), tok::identifier);
    EXPECT_EQ(Tok.getText(), "x");

    // Third token should be '=' operator
    L.lex(Tok);
    EXPECT_EQ(Tok.getKind(), tok::equal);
    EXPECT_EQ(Tok.getText(), "=");

    // Fourth token should be integer literal '42'
    L.lex(Tok);
    EXPECT_EQ(Tok.getKind(), tok::integer_literal);
    EXPECT_EQ(Tok.getText(), "42");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}