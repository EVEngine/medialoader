#pragma once

#include <cstddef>
#include <cstdint>

namespace medialoader {

struct FileHandle;

class FileSystem {
public:
	virtual ~FileSystem() = default;

	virtual FileHandle *open(const char *path, const char *mode) = 0;
	virtual size_t read(FileHandle *h, void *buf, size_t size) = 0;
	virtual bool seek(FileHandle *h, int64_t offset, int whence) = 0;
	virtual int64_t tell(FileHandle *h) = 0;
	virtual int64_t size(FileHandle *h) = 0;
	virtual void close(FileHandle *h) = 0;
	virtual bool exists(const char *path) const = 0;
};

} // namespace medialoader
