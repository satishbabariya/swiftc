/**
 * @file SourceRange.h
 * @brief Defines the SourceRange class which represents a range of source code.
 *
 * This file contains the definition for SourceRange which is used to represent
 * a contiguous range of characters in source code, from a start location to an end location.
 */

#ifndef SWIFT_SOURCE_SOURCERANGE_H
#define SWIFT_SOURCE_SOURCERANGE_H

#include "SourceLocation.h"

namespace swift {
    /**
     * @class SourceRange
     * @brief Represents a contiguous range of characters in source code.
     *
     * A source range is defined by two locations: a start location and an end location.
     * Both locations must be either valid or invalid together.
     */
    class SourceRange {
    public:
        /// The starting location of the range
        SourceLocation Start;

        /// The ending location of the range
        SourceLocation End;

        /// Default constructor creates an invalid range
        SourceRange() = default;

        /**
         * @brief Constructs a zero-length range at the specified location.
         * @param Loc The location for both start and end of the range
         */
        explicit SourceRange(const SourceLocation Loc) : Start(Loc), End(Loc) {
        }

        /**
         * @brief Constructs a range with the specified start and end locations.
         * @param Start The starting location
         * @param End The ending location
         *
         * Start and end must either both be valid or both be invalid.
         */
        SourceRange(const SourceLocation Start, const SourceLocation End) : Start(Start), End(End) {
            assert(Start.isValid() == End.isValid() &&
                "Start and end should either both be valid or both be invalid!");
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
    };
} // namespace swift

#endif // SWIFT_SOURCE_SOURCERANGE_H
