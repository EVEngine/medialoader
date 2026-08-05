#include "medialoader/model/StdFileSystem.h"

#include <cstdio>
#include <cstring>

namespace medialoader {

struct FileHandle {
	FILE *fp;
};

FileHandle *StdFileSystem::open(const char *path, const char *mode) {
	if (!path || !mode)
		return nullptr;
	FILE *fp = std::fopen(path, mode);
	if (!fp)
		return nullptr;
	FileHandle *h = new FileHandle();
	h->fp = fp;
	return h;
}

size_t StdFileSystem::read(FileHandle *h, void *buf, size_t size) {
	if (!h || !h->fp || !buf || size == 0)
		return 0;
	return std::fread(buf, 1, size, h->fp);
}

bool StdFileSystem::seek(FileHandle *h, int64_t offset, int whence) {
	if (!h || !h->fp)
		return false;
#if defined(_MSC_VER)
	return _fseeki64(h->fp, offset, whence) == 0;
#else
	return fseeko(h->fp, static_cast<off_t>(offset), whence) == 0;
#endif
}

int64_t StdFileSystem::tell(FileHandle *h) {
	if (!h || !h->fp)
		return -1;
#if defined(_MSC_VER)
	return _ftelli64(h->fp);
#else
	return static_cast<int64_t>(ftello(h->fp));
#endif
}

int64_t StdFileSystem::size(FileHandle *h) {
	if (!h || !h->fp)
		return -1;
	const int64_t cur = tell(h);
	if (cur < 0)
		return -1;
	if (!seek(h, 0, SEEK_END))
		return -1;
	const int64_t end = tell(h);
	seek(h, cur, SEEK_SET);
	return end;
}

void StdFileSystem::close(FileHandle *h) {
	if (!h)
		return;
	if (h->fp)
		std::fclose(h->fp);
	delete h;
}

bool StdFileSystem::exists(const char *path) const {
	if (!path)
		return false;
	FILE *fp = std::fopen(path, "rb");
	if (!fp)
		return false;
	std::fclose(fp);
	return true;
}

} // namespace medialoader
