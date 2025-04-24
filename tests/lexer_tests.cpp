#include <gtest/gtest.h>
#include <swift/Lexer/Lexer.h>
#include <swift/Lexer/Token.h>
#include <swift/Source/SourceManager.h>

using namespace swift;

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {
        langOpts = LangOptions();
        sourceMgr = std::make_unique<SourceManager>();
    }

    LangOptions langOpts;
    std::unique_ptr<SourceManager> sourceMgr;
    
    // Helper to create a lexer from source code
    std::unique_ptr<Lexer> createLexer(const std::string& source) {
        auto buffer = llvm::MemoryBuffer::getMemBuffer(source);
        unsigned bufferID = sourceMgr->addNewSourceBuffer(std::move(buffer));
        
        return std::make_unique<Lexer>(langOpts, *sourceMgr, bufferID, nullptr,
                                       LexerMode::Swift,
                                       HashbangMode::Allowed,
                                       CommentRetentionMode::ReturnAsTokens);
    }
    
    // Helper to lex a token
    static Token getNextToken(Lexer& lexer) {
        Token tok;
        lexer.lex(tok);
        return tok;
    }
};

TEST_F(LexerTest, TestIdentifier) {
    auto lexer = createLexer("identifier");
    auto token = getNextToken(*lexer);
    
    EXPECT_EQ(token.getKind(), tok::identifier);
    EXPECT_EQ(token.getText(), "identifier");
}

TEST_F(LexerTest, TestIntegerLiteral) {
    auto lexer = createLexer("42");
    auto token = getNextToken(*lexer);
    
    EXPECT_EQ(token.getKind(), tok::integer_literal);
    EXPECT_EQ(token.getText(), "42");
}

TEST_F(LexerTest, TestKeyword) {
    auto lexer = createLexer("func");
    auto token = getNextToken(*lexer);
    
    EXPECT_EQ(token.getKind(), tok::kw_func);
    EXPECT_EQ(token.getText(), "func");
}

TEST_F(LexerTest, TestStringLiteral) {
    auto lexer = createLexer("\"hello world\"");
    auto token = getNextToken(*lexer);
    
    EXPECT_EQ(token.getKind(), tok::string_literal);
    EXPECT_EQ(token.getText(), "\"hello world\"");
}

TEST_F(LexerTest, TestOperator) {
    auto lexer = createLexer("+");
    auto token = getNextToken(*lexer);
    
    EXPECT_EQ(token.getKind(), tok::oper_prefix);
    EXPECT_EQ(token.getText(), "+");
}

TEST_F(LexerTest, TestMultipleTokens) {
    auto lexer = createLexer("let x = 42");
    
    auto token1 = getNextToken(*lexer);
    EXPECT_EQ(token1.getKind(), tok::kw_let);
    
    auto token2 = getNextToken(*lexer);
    EXPECT_EQ(token2.getKind(), tok::identifier);
    EXPECT_EQ(token2.getText(), "x");
    
    auto token3 = getNextToken(*lexer);
    EXPECT_EQ(token3.getKind(), tok::equal);
    
    auto token4 = getNextToken(*lexer);
    EXPECT_EQ(token4.getKind(), tok::integer_literal);
    EXPECT_EQ(token4.getText(), "42");
    
    auto token5 = getNextToken(*lexer);
    EXPECT_EQ(token5.getKind(), tok::eof);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 