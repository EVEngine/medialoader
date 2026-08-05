#pragma once

#include "medialoader/model/FileSystem.h"

namespace medialoader {

class StdFileSystem : public FileSystem {
public:
	FileHandle *open(const char *path, const char *mode) override;
	size_t read(FileHandle *h, void *buf, size_t size) override;
	bool seek(FileHandle *h, int64_t offset, int whence) override;
	int64_t tell(FileHandle *h) override;
	int64_t size(FileHandle *h) override;
	void close(FileHandle *h) override;
	bool exists(const char *path) const override;
};

} // namespace medialoader
