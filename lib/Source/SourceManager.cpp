#include "swift/Source/SourceManager.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

using namespace swift;

SourceManager::SourceManager() {
    // Use the current directory as the working directory.
    FileSystem = llvm::vfs::getRealFileSystem();
}

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

std::optional<unsigned> SourceManager::getIDForBufferIdentifier(llvm::StringRef BufferIdentifier) {
  if (const auto It = BufIdentIDMap.find(BufferIdentifier); It != BufIdentIDMap.end()) {
    return It->second;
  }
  return std::nullopt;
}

const llvm::MemoryBuffer *SourceManager::getMemoryBuffer(unsigned BufferID) const {
  return LLVMSourceMgr.getMemoryBuffer(BufferID);
}

SourceLocation SourceManager::getLocForBufferStart(unsigned BufferID) const {
  return SourceLocation(llvm::SMLoc::getFromPointer(
      LLVMSourceMgr.getMemoryBuffer(BufferID)->getBufferStart()));
}

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

unsigned SourceManager::getLocOffsetInBuffer(SourceLocation Loc, unsigned BufferID) const {
  assert(Loc.isValid() && "location should be valid");
  const auto BufferStart = getLocForBufferStart(BufferID);

  // Check that the location is actually within the specified buffer.
  assert(BufferStart.Value.getPointer() <= Loc.Value.getPointer() &&
         "Location is not from the specified buffer");

  // Return the difference.
  return Loc.Value.getPointer() - BufferStart.Value.getPointer();
}

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

llvm::StringRef SourceManager::getDisplayNameForLoc(SourceLocation Loc) const {
  if (Loc.isInvalid())
    return "<invalid loc>";

  unsigned BufferID = findBufferContainingLoc(Loc);
  return getMemoryBuffer(BufferID)->getBufferIdentifier();
}

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

CharSourceRange SourceManager::getRangeForBuffer(unsigned BufferID) const {
  assert(BufferID != ~0U && "Invalid buffer ID");

  const SourceLocation Start = getLocForBufferStart(BufferID);
  const llvm::StringRef Buffer = getBufferContent(BufferID);

  return CharSourceRange(Start, Buffer.size());
}