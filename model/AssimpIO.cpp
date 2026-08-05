#include "AssimpIO.h"

#include <cstdint>
#include <cstdio>

namespace medialoader {

AssimpIOStream::AssimpIOStream(FileSystem *fs, FileHandle *handle)
    : fs_(fs), handle_(handle) {}

AssimpIOStream::~AssimpIOStream() {
	if (fs_ && handle_) {
		fs_->close(handle_);
		handle_ = nullptr;
	}
}

size_t AssimpIOStream::Read(void *pvBuffer, size_t pSize, size_t pCount) {
	if (!fs_ || !handle_ || pSize == 0)
		return 0;
	const size_t bytes = fs_->read(handle_, pvBuffer, pSize * pCount);
	return bytes / pSize;
}

size_t AssimpIOStream::Write(const void *, size_t, size_t) {
	return 0; // read-only for model loading
}

aiReturn AssimpIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
	if (!fs_ || !handle_)
		return AI_FAILURE;
	int whence = SEEK_SET;
	switch (pOrigin) {
	case aiOrigin_SET: whence = SEEK_SET; break;
	case aiOrigin_CUR: whence = SEEK_CUR; break;
	case aiOrigin_END: whence = SEEK_END; break;
	default: return AI_FAILURE;
	}
	return fs_->seek(handle_, static_cast<int64_t>(pOffset), whence) ? AI_SUCCESS : AI_FAILURE;
}

size_t AssimpIOStream::Tell() const {
	if (!fs_ || !handle_)
		return 0;
	const int64_t t = fs_->tell(handle_);
	return t < 0 ? 0 : static_cast<size_t>(t);
}

size_t AssimpIOStream::FileSize() const {
	if (!fs_ || !handle_)
		return 0;
	const int64_t s = fs_->size(handle_);
	return s < 0 ? 0 : static_cast<size_t>(s);
}

void AssimpIOStream::Flush() {}

AssimpIOSystem::AssimpIOSystem(FileSystem *fs) : fs_(fs) {}

bool AssimpIOSystem::Exists(const char *pFile) const {
	return fs_ && pFile && fs_->exists(pFile);
}

char AssimpIOSystem::getOsSeparator() const {
#if defined(_WIN32)
	return '\\';
#else
	return '/';
#endif
}

Assimp::IOStream *AssimpIOSystem::Open(const char *pFile, const char *pMode) {
	if (!fs_ || !pFile)
		return nullptr;
	const char *mode = (pMode && pMode[0]) ? pMode : "rb";
	FileHandle *h = fs_->open(pFile, mode);
	if (!h)
		return nullptr;
	return new AssimpIOStream(fs_, h);
}

void AssimpIOSystem::Close(Assimp::IOStream *pFile) {
	delete pFile;
}

} // namespace medialoader
