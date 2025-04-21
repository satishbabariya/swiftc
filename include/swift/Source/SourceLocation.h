#ifndef SWIFT_SOURCE_SOURCELOCATION_H
#define SWIFT_SOURCE_SOURCELOCATION_H

#include "llvm/Support/SMLoc.h"

namespace swift {
    class SourceLocation {
    public:
        llvm::SMLoc Value;

        SourceLocation() = default;

        explicit SourceLocation(llvm::SMLoc Value) : Value(Value) {
        }

        [[nodiscard]] bool isValid() const { return Value.isValid(); }
        [[nodiscard]] bool isInvalid() const { return !isValid(); }

        /// An explicit bool operator so one can check if a SourceLoc is valid in an
        /// if statement:
        ///
        /// if (auto x = getSourceLoc()) { ... }
        explicit operator bool() const { return isValid(); }

        bool operator==(const SourceLocation &RHS) const { return RHS.Value == Value; }
        bool operator!=(const SourceLocation &RHS) const { return !operator==(RHS); }

        /// Return a source location advanced a specified number of bytes.
        [[nodiscard]] SourceLocation getAdvancedLoc(const int ByteOffset) const {
            assert(isValid() && "Can't advance an invalid location");
            return SourceLocation(
                llvm::SMLoc::getFromPointer(Value.getPointer() + ByteOffset));
        }

        [[nodiscard]] SourceLocation getAdvancedLocOrInvalid(const int ByteOffset) const {
            if (isValid())
                return getAdvancedLoc(ByteOffset);
            return {};
        }

        [[nodiscard]] const void *getOpaquePointerValue() const { return Value.getPointer(); }
    };
} // namespace swift

#endif // SWIFT_SOURCE_SOURCELOCATION_H
