#ifndef SWIFT_SOURCE_CHAR_SOURCE_RANGE_H
#define SWIFT_SOURCE_CHAR_SOURCE_RANGE_H

#include "SourceLocation.h"

namespace swift {
    class CharSourceRange {
    public:
        /// The beginning location of the range.
        SourceLocation Start;
        /// The length of the range.
        unsigned Length{};

        CharSourceRange() = default;

        CharSourceRange(const SourceLocation Start, const unsigned Length)
            : Start(Start), Length(Length) {
        }

        CharSourceRange(SourceLocation Start, SourceLocation End)
            : Start(Start), Length(End.isValid()
                                       ? End.Value.getPointer() -
                                         Start.Value.getPointer()
                                       : 0) {
            assert(Start.isValid() == End.isValid() &&
                "Start and end should either both be valid or both be invalid!");
        }

        static CharSourceRange getTokenRange(SourceLocation Start, SourceLocation End) {
            return {Start, End};
        }

        static CharSourceRange getCharRange(SourceLocation Start, SourceLocation End) {
            return {
                Start, End.getOpaquePointerValue() !=
                       Start.getOpaquePointerValue()
                           ? End.getAdvancedLoc(1)
                           : End
            };
        }

        [[nodiscard]] bool isValid() const { return Start.isValid(); }
        [[nodiscard]] bool isInvalid() const { return Start.isInvalid(); }

        [[nodiscard]] SourceLocation getStart() const { return Start; }

        [[nodiscard]] SourceLocation getEnd() const {
            if (Length == 0)
                return Start;
            return SourceLocation(llvm::SMLoc::getFromPointer(
                Start.Value.getPointer() + Length - 1));
        }

        /// Returns the byte length of this range.
        [[nodiscard]] unsigned getByteLength() const { return Length; }
    };
} // namespace swift

#endif // SWIFT_SOURCE_CHAR_SOURCE_RANGE_H
