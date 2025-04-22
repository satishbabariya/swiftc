/**
 * @file SourceLocation.h
 * @brief Defines the SourceLocation class which represents a position in source code.
 *
 * This file contains the definition for SourceLocation which is used throughout
 * the Swift compiler to represent positions in source code files.
 */

#ifndef SWIFT_SOURCE_SOURCELOCATION_H
#define SWIFT_SOURCE_SOURCELOCATION_H

#include "llvm/Support/SMLoc.h"

namespace swift {
    /**
     * @class SourceLocation
     * @brief Represents a specific location in source code.
     *
     * SourceLocation is a lightweight wrapper around LLVM's SMLoc class.
     * It represents a specific position in a source file and provides
     * utilities for working with these positions.
     */
    class SourceLocation {
    public:
        /// The underlying LLVM source location value
        llvm::SMLoc Value;

        /// Default constructor creates an invalid location
        SourceLocation() = default;

        /**
         * @brief Constructs a SourceLocation from an LLVM SMLoc.
         * @param Value The SMLoc value to wrap
         */
        explicit SourceLocation(llvm::SMLoc Value) : Value(Value) {
        }

        /**
         * @brief Checks if this location is valid.
         * @return True if the location is valid
         */
        [[nodiscard]] bool isValid() const { return Value.isValid(); }
        
        /**
         * @brief Checks if this location is invalid.
         * @return True if the location is invalid
         */
        [[nodiscard]] bool isInvalid() const { return !isValid(); }

        /**
         * @brief An explicit bool operator to check validity in if statements.
         * 
         * Allows code like: if (auto x = getSourceLoc()) { ... }
         * @return True if the location is valid
         */
        explicit operator bool() const { return isValid(); }

        /**
         * @brief Equality comparison operator.
         * @param RHS Right-hand side to compare with
         * @return True if locations are equal
         */
        bool operator==(const SourceLocation &RHS) const { return RHS.Value == Value; }
        
        /**
         * @brief Inequality comparison operator.
         * @param RHS Right-hand side to compare with
         * @return True if locations are not equal
         */
        bool operator!=(const SourceLocation &RHS) const { return !operator==(RHS); }

        /**
         * @brief Returns a source location advanced by the specified number of bytes.
         * @param ByteOffset Number of bytes to advance
         * @return A new source location advanced by ByteOffset
         */
        [[nodiscard]] SourceLocation getAdvancedLoc(const int ByteOffset) const {
            assert(isValid() && "Can't advance an invalid location");
            return SourceLocation(
                llvm::SMLoc::getFromPointer(Value.getPointer() + ByteOffset));
        }

        /**
         * @brief Returns a source location advanced by the specified number of bytes or invalid if this location is invalid.
         * @param ByteOffset Number of bytes to advance
         * @return A new source location advanced by ByteOffset, or an invalid location if this location is invalid
         */
        [[nodiscard]] SourceLocation getAdvancedLocOrInvalid(const int ByteOffset) const {
            if (isValid())
                return getAdvancedLoc(ByteOffset);
            return {};
        }

        /**
         * @brief Returns the opaque pointer value of this location.
         * @return Opaque pointer value
         */
        [[nodiscard]] const void *getOpaquePointerValue() const { return Value.getPointer(); }
    };
} // namespace swift

#endif // SWIFT_SOURCE_SOURCELOCATION_H
