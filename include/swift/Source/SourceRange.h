#ifndef SWIFT_SOURCE_SOURCERANGE_H
#define SWIFT_SOURCE_SOURCERANGE_H

#include "SourceLocation.h"

namespace swift {
    class SourceRange {
    public:
        SourceLocation Start, End;

        SourceRange() = default;

        explicit SourceRange(const SourceLocation Loc) : Start(Loc), End(Loc) {
        }

        SourceRange(const SourceLocation Start, const SourceLocation End) : Start(Start), End(End) {
            assert(Start.isValid() == End.isValid() &&
                "Start and end should either both be valid or both be invalid!");
        }

        [[nodiscard]] bool isValid() const { return Start.isValid(); }
        [[nodiscard]] bool isInvalid() const { return Start.isInvalid(); }
    };
} // namespace swift

#endif // SWIFT_SOURCE_SOURCERANGE_H
