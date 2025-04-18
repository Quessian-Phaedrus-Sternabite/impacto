#pragma once

#include "stream.h"
#include "../util.h"
#include <ankerl/unordered_dense.h>

namespace Impacto {
namespace Io {

class VfsArchive {
 public:
  virtual ~VfsArchive();

  // Meta.ArchiveFileName, Meta.ArchiveMountPoint, Meta.FileName are set by VFS,
  // not by the archiver.
  // These methods are only ever called with FileMeta* found in IdsToFiles.
  virtual IoError Open(FileMeta* file, Stream** outStream) = 0;

  virtual IoError Slurp(FileMeta* file, void*& outBuffer, int64_t& outSize);
  // If the size of a file is uncertain when the archive is first opened (e.g.
  // directory-listing archives), the file's size in IdsToFiles must be negative
  // and this must be overridden
  virtual IoError GetCurrentSize(FileMeta* file, int64_t& outSize);

  ankerl::unordered_dense::map<std::string, uint32_t, string_hash,
                               std::equal_to<>>
      NamesToIds;
  ankerl::unordered_dense::map<uint32_t, FileMeta*> IdsToFiles;

  std::string MountPoint;

  bool IsInit = false;
  Stream* BaseStream = 0;
};

}  // namespace Io
}  // namespace Impacto