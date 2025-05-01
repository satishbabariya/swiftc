//
// Created by Satish Babariya on 23/04/25.
//

#ifndef LEXERSTATE_H
#define LEXERSTATE_H

#include "swift/Source/SourceManager.h"

namespace swift {
    /// Lexer state can be saved/restored to/from objects of this class.
    class LexerState {
    public:
        LexerState() : Loc() {
        }

        bool isValid() const { return Loc.isValid(); }

        LexerState advance(unsigned Offset) const {
            assert(isValid());
            return LexerState(Loc.getAdvancedLoc(Offset));
        }

    private:
        explicit LexerState(SourceLocation Loc) : Loc(Loc) {
        }

        SourceLocation Loc;
        friend class Lexer;
    };
}

#endif //LEXERSTATE_H
