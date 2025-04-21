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
    /// Manages source buffers and provides utilities for working with source locations.
    class SourceManager {
    public:
        /// Default constructor.
        SourceManager();

        /// Adds a memory buffer to the SourceManager, returning the buffer ID.
        unsigned addNewSourceBuffer(std::unique_ptr<llvm::MemoryBuffer> Buffer);

        /// Returns a buffer ID for the specified <file, buffer> pair.
        /// If the buffer is not already added, it gets added.
        /// If the buffer cannot be read, or already exists with different contents,
        /// this returns ~0U.
        unsigned getOrOpenBuffer(llvm::StringRef FilePath);

        /// Returns the buffer ID for an existing buffer if it exists.
        std::optional<unsigned> getIDForBufferIdentifier(llvm::StringRef BufferIdentifier);

        /// Returns the memory buffer for the provided buffer ID.
        [[nodiscard]] const llvm::MemoryBuffer *getMemoryBuffer(unsigned BufferID) const;

        /// Returns a StringRef to the contents of the buffer with the given ID.
        [[nodiscard]] llvm::StringRef getBufferContent(unsigned BufferID) const {
            return getMemoryBuffer(BufferID)->getBuffer();
        }

        /// Returns the source location for the beginning of the specified buffer.
        [[nodiscard]] SourceLocation getLocForBufferStart(unsigned BufferID) const;

        /// Returns the source buffer ID for the given location.
        [[nodiscard]] unsigned findBufferContainingLoc(SourceLocation Loc) const;

        /// Returns the offset in bytes for the given source location.
        [[nodiscard]] unsigned getLocOffsetInBuffer(SourceLocation Loc, unsigned BufferID) const;

        /// Returns the distance in bytes between the given source locations.
        [[nodiscard]] unsigned getByteDistance(SourceLocation Start, SourceLocation End) const;

        /// Returns the SourceLoc for the byte offset in the specified buffer.
        [[nodiscard]] SourceLocation getLocForOffset(const unsigned BufferID, const unsigned Offset) const {
            return getLocForBufferStart(BufferID).getAdvancedLoc(Offset);
        }

        /// Returns a buffer identifier suitable for display to the user.
        [[nodiscard]] llvm::StringRef getDisplayNameForLoc(SourceLocation Loc) const;

        /// Returns true if \c LHS is before \c RHS in the same source buffer.
        [[nodiscard]] static bool isBeforeInBuffer(const SourceLocation LHS, const SourceLocation RHS) {
            return LHS.Value.getPointer() < RHS.Value.getPointer();
        }

        /// Returns true if range \c R contains the location \c Loc.
        [[nodiscard]] bool containsLoc(SourceRange Range, SourceLocation Loc) const;

        /// Extract the full content of the source buffer with the given ID.
        [[nodiscard]] llvm::StringRef extractText(const unsigned BufferID) const {
            return getMemoryBuffer(BufferID)->getBuffer();
        }

        /// Extract the source text for a range.
        [[nodiscard]] llvm::StringRef extractText(CharSourceRange Range) const;

        /// Returns the source range for the entire buffer.
        [[nodiscard]] CharSourceRange getRangeForBuffer(unsigned BufferID) const;

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
