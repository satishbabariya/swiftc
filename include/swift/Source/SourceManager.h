/**
 * @file SourceManager.h
 * @brief Defines the SourceManager class which handles source buffers and locations.
 *
 * This file contains definitions for managing source code buffers, handling source
 * locations, and providing utilities for working with source ranges.
 */

#ifndef SWIFT_SOURCE_SOURCE_MANAGER_H
#define SWIFT_SOURCE_SOURCE_MANAGER_H

#include "SourceLocation.h"
#include "SourceRange.h"
#include "CharSourceRange.h"

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/VirtualFileSystem.h"

namespace swift {
    /**
     * @class SourceManager
     * @brief Manages source buffers and provides utilities for working with source locations.
     *
     * SourceManager is responsible for:
     * - Managing source code buffers
     * - Creating and resolving source locations
     * - Handling source ranges
     * - Providing source text extraction utilities
     */
    class SourceManager {
    public:
        /**
         * @brief Default constructor.
         */
        SourceManager();

        /**
         * @brief Adds a memory buffer to the SourceManager.
         * @param Buffer Memory buffer to add
         * @return Buffer ID for the added buffer
         */
        unsigned addNewSourceBuffer(std::unique_ptr<llvm::MemoryBuffer> Buffer);

        /**
         * @brief Returns a buffer ID for the specified file path.
         * @param FilePath Path to the file
         * @return Buffer ID if successful, ~0U otherwise
         *
         * If the buffer is not already added, it gets added.
         * If the buffer cannot be read, or already exists with different contents,
         * this returns ~0U.
         */
        std::optional<unsigned> getOrOpenBuffer(llvm::StringRef FilePath);

        /**
         * @brief Returns the buffer ID for an existing buffer if it exists.
         * @param BufferIdentifier The identifier for the buffer
         * @return Optional buffer ID, empty if buffer doesn't exist
         */
        std::optional<unsigned> getIDForBufferIdentifier(llvm::StringRef BufferIdentifier);

        /**
         * @brief Returns the memory buffer for the provided buffer ID.
         * @param BufferID ID of the buffer to retrieve
         * @return Pointer to the memory buffer
         */
        [[nodiscard]] const llvm::MemoryBuffer *getMemoryBuffer(unsigned BufferID) const;

        /**
         * @brief Returns a StringRef to the contents of the buffer with the given ID.
         * @param BufferID ID of the buffer
         * @return StringRef to buffer contents
         */
        [[nodiscard]] llvm::StringRef getBufferContent(unsigned BufferID) const {
            return getMemoryBuffer(BufferID)->getBuffer();
        }

        /**
         * @brief Returns the source location for the beginning of the specified buffer.
         * @param BufferID ID of the buffer
         * @return Source location for the buffer's start
         */
        [[nodiscard]] SourceLocation getLocForBufferStart(unsigned BufferID) const;

        /**
         * @brief Returns the source buffer ID for the given location.
         * @param Loc Source location to find buffer for
         * @return ID of the buffer containing the location
         */
        [[nodiscard]] unsigned findBufferContainingLoc(SourceLocation Loc) const;

        /**
         * @brief Returns the offset in bytes for the given source location.
         * @param Loc Source location to get offset for
         * @param BufferID ID of the buffer containing the location
         * @return Offset in bytes from buffer start
         */
        [[nodiscard]] unsigned getLocOffsetInBuffer(SourceLocation Loc, unsigned BufferID) const;

        /**
         * @brief Returns the distance in bytes between the given source locations.
         * @param Start Starting source location
         * @param End Ending source location
         * @return Distance in bytes between locations
         */
        [[nodiscard]] unsigned getByteDistance(SourceLocation Start, SourceLocation End) const;

        /**
         * @brief Returns the SourceLoc for the byte offset in the specified buffer.
         * @param BufferID ID of the buffer
         * @param Offset Byte offset within the buffer
         * @return Source location corresponding to the offset
         */
        [[nodiscard]] SourceLocation getLocForOffset(const unsigned BufferID, const unsigned Offset) const {
            return getLocForBufferStart(BufferID).getAdvancedLoc(Offset);
        }

        /**
         * @brief Returns a buffer identifier suitable for display to the user.
         * @param Loc Source location to get display name for
         * @return Display name as a StringRef
         */
        [[nodiscard]] llvm::StringRef getDisplayNameForLoc(SourceLocation Loc) const;

        /**
         * @brief Returns true if LHS is before RHS in the same source buffer.
         * @param LHS First source location
         * @param RHS Second source location
         * @return True if LHS is before RHS
         */
        [[nodiscard]] static bool isBeforeInBuffer(const SourceLocation LHS, const SourceLocation RHS) {
            return LHS.Value.getPointer() < RHS.Value.getPointer();
        }

        /**
         * @brief Returns true if range contains the location.
         * @param Range Source range to check
         * @param Loc Location to check for containment
         * @return True if the range contains the location
         */
        [[nodiscard]] bool containsLoc(SourceRange Range, SourceLocation Loc) const;

        /**
         * @brief Extract the full content of the source buffer with the given ID.
         * @param BufferID ID of the buffer to extract from
         * @return StringRef to the buffer content
         */
        [[nodiscard]] llvm::StringRef extractText(const unsigned BufferID) const {
            return getMemoryBuffer(BufferID)->getBuffer();
        }

        /**
         * @brief Extract the source text for a range.
         * @param Range Character source range to extract
         * @return StringRef to the extracted text
         */
        [[nodiscard]] llvm::StringRef extractText(CharSourceRange Range) const;

        /**
         * @brief Returns the source range for the entire buffer.
         * @param BufferID ID of the buffer
         * @return Character source range for the entire buffer
         */
        [[nodiscard]] CharSourceRange getRangeForBuffer(unsigned BufferID) const;

        unsigned addMemBufferCopy(const llvm::MemoryBuffer *Buffer);

        unsigned addMemBufferCopy(llvm::StringRef InputData, llvm::StringRef BufIdentifier);

    private:
        /// LLVM SourceMgr that handles the raw source buffers.
        llvm::SourceMgr LLVMSourceMgr;

        /// Virtual file system for accessing source files.
        llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> FileSystem;

        /// Associates buffer identifiers to buffer IDs.
        llvm::DenseMap<llvm::StringRef, unsigned> BufIdentIDMap;
    };
} // namespace swift

#endif
