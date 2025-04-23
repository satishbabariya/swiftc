/**
 * @file SourceManager.cpp
 * @brief Implementation of the SourceManager class.
 *
 * This file contains the implementation of the SourceManager class, which
 * provides functionality for managing source buffers and working with source locations.
 */

#include "swift/Source/SourceManager.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

using namespace swift;

/**
 * Default constructor for SourceManager.
 * Initializes the file system to use the real file system.
 */
SourceManager::SourceManager() {
    // Use the current directory as the working directory.
    FileSystem = llvm::vfs::getRealFileSystem();
}

/**
 * Adds a new source buffer to the SourceManager.
 * 
 * @param Buffer The memory buffer to add
 * @return The buffer ID for the added buffer
 */
unsigned SourceManager::addNewSourceBuffer(std::unique_ptr<llvm::MemoryBuffer> Buffer) {
  const llvm::StringRef BufferIdentifier = Buffer->getBufferIdentifier();

  // Check if we already have this buffer. If so, just return the ID.
  auto ExistingBuffer = getIDForBufferIdentifier(BufferIdentifier);
  if (ExistingBuffer.has_value()) {
    return *ExistingBuffer;
  }

  // Add the buffer to LLVM's SourceMgr.
  unsigned BufferID = LLVMSourceMgr.AddNewSourceBuffer(std::move(Buffer), llvm::SMLoc());

  // Remember the buffer identifier.
  BufIdentIDMap[BufferIdentifier] = BufferID;

  return BufferID;
}

/**
 * Returns a buffer ID for the specified file path.
 * If the buffer is not already added, it gets added.
 * 
 * @param FilePath Path to the file
 * @return Buffer ID if successful, ~0U otherwise
 */
unsigned SourceManager::getOrOpenBuffer(llvm::StringRef FilePath) {
  // Check if we already have this buffer.
  if (const auto ExistingBuffer = getIDForBufferIdentifier(FilePath); ExistingBuffer.has_value()) {
    return *ExistingBuffer;
  }

  // Otherwise, create and add the buffer.
  auto FileOrErr = FileSystem->getBufferForFile(FilePath);
  if (!FileOrErr) {
    // Return an invalid BufferID on error.
    return ~0U;
  }

  return addNewSourceBuffer(std::move(*FileOrErr));
}

/**
 * Returns the buffer ID for an existing buffer if it exists.
 * 
 * @param BufferIdentifier The identifier for the buffer
 * @return Optional buffer ID, empty if buffer doesn't exist
 */
std::optional<unsigned> SourceManager::getIDForBufferIdentifier(llvm::StringRef BufferIdentifier) {
  if (const auto It = BufIdentIDMap.find(BufferIdentifier); It != BufIdentIDMap.end()) {
    return It->second;
  }
  return std::nullopt;
}

/**
 * Returns the memory buffer for the provided buffer ID.
 * 
 * @param BufferID ID of the buffer to retrieve
 * @return Pointer to the memory buffer
 */
const llvm::MemoryBuffer *SourceManager::getMemoryBuffer(unsigned BufferID) const {
  return LLVMSourceMgr.getMemoryBuffer(BufferID);
}

/**
 * Returns the source location for the beginning of the specified buffer.
 * 
 * @param BufferID ID of the buffer
 * @return Source location for the buffer's start
 */
SourceLocation SourceManager::getLocForBufferStart(unsigned BufferID) const {
  return SourceLocation(llvm::SMLoc::getFromPointer(
      LLVMSourceMgr.getMemoryBuffer(BufferID)->getBufferStart()));
}

/**
 * Returns the source buffer ID for the given location.
 * 
 * @param Loc Source location to find buffer for
 * @return ID of the buffer containing the location
 */
unsigned SourceManager::findBufferContainingLoc(SourceLocation Loc) const {
  assert(Loc.isValid() && "location should be valid");

  // Search through all the buffers to find which one contains this location.
  for (unsigned i = 1, e = LLVMSourceMgr.getNumBuffers() + 1; i != e; ++i) {
    if (auto *Buffer = LLVMSourceMgr.getMemoryBuffer(i); Buffer->getBufferStart() <= Loc.Value.getPointer() &&
                                                         Loc.Value.getPointer() <= Buffer->getBufferEnd()) {
      return i;
    }
  }

  llvm_unreachable("Location not in any buffer");
}

/**
 * Returns the offset in bytes for the given source location within the specified buffer.
 * 
 * @param Loc Source location to get offset for
 * @param BufferID ID of the buffer containing the location
 * @return Offset in bytes from buffer start
 */
unsigned SourceManager::getLocOffsetInBuffer(SourceLocation Loc, unsigned BufferID) const {
  assert(Loc.isValid() && "location should be valid");
  const auto BufferStart = getLocForBufferStart(BufferID);

  // Check that the location is actually within the specified buffer.
  assert(BufferStart.Value.getPointer() <= Loc.Value.getPointer() &&
         "Location is not from the specified buffer");

  // Return the difference.
  return Loc.Value.getPointer() - BufferStart.Value.getPointer();
}

/**
 * Returns the distance in bytes between the given source locations.
 * 
 * @param Start Starting source location
 * @param End Ending source location
 * @return Distance in bytes between locations
 */
unsigned SourceManager::getByteDistance(SourceLocation Start, SourceLocation End) const {
  assert(Start.isValid() && End.isValid() && "locations should be valid");

  const unsigned Buffer1 = findBufferContainingLoc(Start);
  const unsigned Buffer2 = findBufferContainingLoc(End);

  // If the buffers are the same, just do the math.
  if (Buffer1 == Buffer2) {
    return End.Value.getPointer() - Start.Value.getPointer();
  }

  // Otherwise, the locations are in different buffers.
  // This isn't common, but when it happens we compute the distance
  // conservatively: we assume the buffers are arranged sequentially.
  unsigned Distance = 0;

  // Add the distance from Start to the end of its buffer.
  auto *Buffer1Data = getMemoryBuffer(Buffer1);
  Distance += (Buffer1Data->getBufferEnd() - Start.Value.getPointer());

  // Add the distances of any intermediate buffers.
  for (unsigned i = Buffer1 + 1; i != Buffer2; ++i) {
    auto *BufferData = getMemoryBuffer(i);
    Distance += BufferData->getBufferSize();
  }

  // Add the distance from the start of Buffer2 to End.
  Distance += (End.Value.getPointer() -
               getMemoryBuffer(Buffer2)->getBufferStart());

  return Distance;
}

/**
 * Returns a buffer identifier suitable for display to the user.
 * 
 * @param Loc Source location to get display name for
 * @return Display name as a StringRef
 */
llvm::StringRef SourceManager::getDisplayNameForLoc(SourceLocation Loc) const {
  if (Loc.isInvalid())
    return "<invalid loc>";

  unsigned BufferID = findBufferContainingLoc(Loc);
  return getMemoryBuffer(BufferID)->getBufferIdentifier();
}

/**
 * Returns true if range contains the location.
 * 
 * @param Range Source range to check
 * @param Loc Location to check for containment
 * @return True if the range contains the location
 */
bool SourceManager::containsLoc(SourceRange Range, SourceLocation Loc) const {
  assert(Range.isValid() && "Range should be valid");
  assert(Loc.isValid() && "Location should be valid");

  // If the location and range are in different buffers, they can't overlap.
  if (const unsigned BufferID = findBufferContainingLoc(Range.Start); BufferID != findBufferContainingLoc(Loc))
    return false;

  // Check if the location is within the range.
  return Range.Start.Value.getPointer() <= Loc.Value.getPointer() &&
         Loc.Value.getPointer() <= Range.End.Value.getPointer();
}

/**
 * Extract the source text for a range.
 * 
 * @param Range Character source range to extract
 * @return StringRef to the extracted text
 */
llvm::StringRef SourceManager::extractText(CharSourceRange Range) const {
  assert(Range.isValid() && "Range should be valid");

  const unsigned BufferID = findBufferContainingLoc(Range.getStart());
  if (BufferID == ~0U)
    return {};

  const unsigned StartOffset = getLocOffsetInBuffer(Range.getStart(), BufferID);
  unsigned EndOffset;

  if (Range.getEnd().isValid()) {
    EndOffset = getLocOffsetInBuffer(Range.getEnd(), BufferID);
  } else {
    EndOffset = StartOffset + Range.getByteLength();
  }

  const llvm::StringRef Buffer = getBufferContent(BufferID);
  return Buffer.substr(StartOffset, EndOffset - StartOffset);
}

/**
 * Returns the source range for the entire buffer.
 * 
 * @param BufferID ID of the buffer
 * @return Character source range for the entire buffer
 */
CharSourceRange SourceManager::getRangeForBuffer(unsigned BufferID) const {
  assert(BufferID != ~0U && "Invalid buffer ID");

  const SourceLocation Start = getLocForBufferStart(BufferID);
  const llvm::StringRef Buffer = getBufferContent(BufferID);

  return CharSourceRange(Start, Buffer.size());
}