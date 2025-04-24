/**
 * @file CharSourceRange.h
 * @brief Defines the CharSourceRange class which represents a character-based range in source code.
 *
 * This file contains the definition for CharSourceRange which represents a
 * range of characters in source code, specified by a start location and a length.
 */

#ifndef SWIFT_SOURCE_CHAR_SOURCE_RANGE_H
#define SWIFT_SOURCE_CHAR_SOURCE_RANGE_H

#include "SourceLocation.h"

namespace swift {
    /**
     * @class CharSourceRange
     * @brief Represents a character-based range in source code.
     *
     * Unlike SourceRange which uses a start and end location, CharSourceRange
     * uses a start location and a length in bytes. This allows for more
     * precise handling of character-based operations.
     */
    class CharSourceRange {
    public:
        /// The beginning location of the range.
        SourceLocation Start;
        /// The length of the range in bytes.
        unsigned Length{};

        /// Default constructor creates an invalid range
        CharSourceRange() = default;

        /**
         * @brief Constructs a range with the specified start location and length.
         * @param Start The starting location
         * @param Length The length of the range in bytes
         */
        CharSourceRange(const SourceLocation Start, const unsigned Length)
            : Start(Start), Length(Length) {
        }

        /**
         * @brief Constructs a range with the specified start and end locations.
         * @param Start The starting location
         * @param End The ending location
         *
         * The length is computed as the byte distance between Start and End.
         * Start and end must either both be valid or both be invalid.
         */
        CharSourceRange(SourceLocation Start, SourceLocation End)
            : Start(Start), Length(End.isValid()
                                       ? End.Value.getPointer() -
                                         Start.Value.getPointer()
                                       : 0) {
            assert(Start.isValid() == End.isValid() &&
                "Start and end should either both be valid or both be invalid!");
        }

        /**
         * @brief Creates a token range from start to end.
         * @param Start The starting location
         * @param End The ending location
         * @return A CharSourceRange representing the token range
         */
        static CharSourceRange getTokenRange(SourceLocation Start, SourceLocation End) {
            return {Start, End};
        }

        /**
         * @brief Creates a character range from start to end.
         * @param Start The starting location
         * @param End The ending location
         * @return A CharSourceRange representing the character range
         *
         * If Start and End point to the same location, the range will include only
         * that location. Otherwise, the range will include the character at End.
         */
        static CharSourceRange getCharRange(SourceLocation Start, SourceLocation End) {
            return {
                Start, End.getOpaquePointerValue() !=
                       Start.getOpaquePointerValue()
                           ? End.getAdvancedLoc(1)
                           : End
            };
        }

        /**
         * @brief Checks if this range is valid.
         * @return True if the range is valid
         */
        [[nodiscard]] bool isValid() const { return Start.isValid(); }

        /**
         * @brief Checks if this range is invalid.
         * @return True if the range is invalid
         */
        [[nodiscard]] bool isInvalid() const { return Start.isInvalid(); }

        /**
         * @brief Gets the starting location of the range.
         * @return The starting location
         */
        [[nodiscard]] SourceLocation getStart() const { return Start; }

        /**
         * @brief Gets the ending location of the range.
         * @return The ending location
         *
         * If the length is 0, returns the start location.
         * Otherwise, returns a location that is (Length - 1) bytes after the start.
         */
        [[nodiscard]] SourceLocation getEnd() const {
            if (Length == 0)
                return Start;
            return SourceLocation(llvm::SMLoc::getFromPointer(
                Start.Value.getPointer() + Length - 1));
        }

        /**
         * @brief Returns the byte length of this range.
         * @return The length in bytes
         */
        [[nodiscard]] unsigned getByteLength() const { return Length; }
    };
} // namespace swift

#endif // SWIFT_SOURCE_CHAR_SOURCE_RANGE_H
