//
// Created by Satish Babariya on 25/04/25.
//

#include "swift/Lexer/Lexer.h"
#include <set>

static void getStringPartTokens(const swift::Token &Tok,
                                const swift::LangOptions &LangOpts,
                                const swift::SourceManager &SM, int BufID,
                                std::vector<swift::Token> &Toks);

namespace swift {
    template <typename DF>
    void tokenize(const LangOptions &LangOpts, const SourceManager &SM,
                  unsigned BufferID, unsigned Offset, unsigned EndOffset,
                  DiagnosticEngine * Diags,
                  CommentRetentionMode RetainComments,
                  bool TokenizeInterpolatedString, llvm::ArrayRef<Token> SplitTokens,
                  DF &&DestFunc) {
        if (Offset == 0 && EndOffset == 0)
            EndOffset = SM.getRangeForBuffer(BufferID).getByteLength();

        Lexer L(LangOpts, SM, BufferID, Diags, LexerMode::Swift,
                HashbangMode::Allowed, RetainComments, Offset,
                EndOffset);

        auto TokComp = [&](const Token &A, const Token &B) {
            return SM.isBeforeInBuffer(A.getLoc(), B.getLoc());
        };

        std::set<Token, decltype(TokComp)> ResetTokens(TokComp);
        for (auto C = SplitTokens.begin(), E = SplitTokens.end(); C != E; ++C) {
            ResetTokens.insert(*C);
        }

        Token Tok;
        do {
            L.lex(Tok);

            // If the token has the same location as a reset location,
            // reset the token stream
            auto F = ResetTokens.find(Tok);
            if (F != ResetTokens.end()) {
                assert(F->isNot(tok::string_literal));

                DestFunc(*F);

                auto NewState = L.getStateForBeginningOfTokenLoc(
                    F->getLoc().getAdvancedLoc(F->getLength()));
                L.restoreState(NewState);
                continue;
            }

            if (Tok.is(tok::string_literal) && TokenizeInterpolatedString) {
                std::vector<Token> StrTokens;
                getStringPartTokens(Tok, LangOpts, SM, BufferID, StrTokens);
                for (auto &StrTok : StrTokens) {
                    DestFunc(StrTok);
                }
            } else {
                DestFunc(Tok);
            }

        } while (Tok.getKind() != tok::eof);
    }
} // namespace swift

using namespace swift;

/// Tokenizes a string literal, taking into account string interpolation.
static void getStringPartTokens(const Token &Tok, const LangOptions &LangOpts,
                                const SourceManager &SM,
                                int BufID, std::vector<Token> &Toks) {
  assert(Tok.is(tok::string_literal));
  bool IsMultiline = Tok.isMultilineString();
  unsigned CustomDelimiterLen = Tok.getCustomDelimiterLen();
  unsigned QuoteLen = (IsMultiline ? 3 : 1) + CustomDelimiterLen;
  llvm::SmallVector<Lexer::StringSegment, 4> Segments;
  Lexer::getStringLiteralSegments(Tok, Segments, /*Diags=*/nullptr);
  for (unsigned i = 0, e = Segments.size(); i != e; ++i) {
    Lexer::StringSegment &Seg = Segments[i];
    bool isFirst = i == 0;
    bool isLast = i == e-1;
    if (Seg.Kind == Lexer::StringSegment::Literal) {
      SourceLocation Loc = Seg.Loc;
      unsigned Len = Seg.Length;
      if (isFirst) {
        // Include the quote.
        Loc = Loc.getAdvancedLoc(-QuoteLen);
        Len += QuoteLen;
      }
      if (isLast) {
        // Include the quote.
        Len += QuoteLen;
      }

      llvm::StringRef Text = SM.extractText({ Loc, Len });
      Token NewTok;
      NewTok.setToken(tok::string_literal, Text);
      NewTok.setStringLiteral(IsMultiline, CustomDelimiterLen);
      Toks.push_back(NewTok);

    } else {
      assert(Seg.Kind == Lexer::StringSegment::Expr &&
             "new enumerator was introduced ?");
      unsigned Offset = SM.getLocOffsetInBuffer(Seg.Loc, BufID);
      unsigned EndOffset = Offset + Seg.Length;

      if (isFirst) {
        // Add a token for the quote character.
        llvm::StringRef Text = SM.extractText({ Seg.Loc.getAdvancedLoc(-2), 1 });
        Token NewTok;
        NewTok.setToken(tok::string_literal, Text);
        Toks.push_back(NewTok);
      }

      std::vector<Token> NewTokens = swift::tokenize(LangOpts, SM, BufID,
                                                     Offset, EndOffset,
                                                     /*Diags=*/nullptr,
                                                     /*KeepComments=*/true);
      Toks.insert(Toks.end(), NewTokens.begin(), NewTokens.end());

      if (isLast) {
        // Add a token for the quote character.
        llvm::StringRef Text = SM.extractText({ Seg.Loc.getAdvancedLoc(Seg.Length),
                                          1 });
        Token NewTok;
        NewTok.setToken(tok::string_literal, Text);
        Toks.push_back(NewTok);
      }
    }
  }
}

std::vector<Token> swift::tokenize(const LangOptions &LangOpts,
                                   const SourceManager &SM, unsigned BufferID,
                                   unsigned Offset, unsigned EndOffset,
                                   DiagnosticEngine *Diags,
                                   bool KeepComments,
                                   bool TokenizeInterpolatedString,
                                   llvm::ArrayRef<Token> SplitTokens) {
  std::vector<Token> Tokens;

  tokenize(LangOpts, SM, BufferID, Offset, EndOffset, Diags,
           KeepComments ? CommentRetentionMode::ReturnAsTokens
                        : CommentRetentionMode::AttachToNextToken,
           TokenizeInterpolatedString,
           SplitTokens,
           [&](const Token &Tok) { Tokens.push_back(Tok); });

  assert(Tokens.back().is(tok::eof));
  Tokens.pop_back(); // Remove EOF.
  return Tokens;
}