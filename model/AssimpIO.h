#pragma once

#include "medialoader/model/FileSystem.h"

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

namespace medialoader {

class AssimpIOStream : public Assimp::IOStream {
public:
	AssimpIOStream(FileSystem *fs, FileHandle *handle);
	~AssimpIOStream() override;

	size_t Read(void *pvBuffer, size_t pSize, size_t pCount) override;
	size_t Write(const void *pvBuffer, size_t pSize, size_t pCount) override;
	aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;
	size_t Tell() const override;
	size_t FileSize() const override;
	void Flush() override;

private:
	FileSystem *fs_;
	FileHandle *handle_;
};

class AssimpIOSystem : public Assimp::IOSystem {
public:
	explicit AssimpIOSystem(FileSystem *fs);

	bool Exists(const char *pFile) const override;
	char getOsSeparator() const override;
	Assimp::IOStream *Open(const char *pFile, const char *pMode = "rb") override;
	void Close(Assimp::IOStream *pFile) override;

private:
	FileSystem *fs_;
};

} // namespace medialoader
