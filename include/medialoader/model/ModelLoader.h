#pragma once

#include "medialoader/model/FileSystem.h"
#include "medialoader/model/ModelScene.h"
#include "medialoader/model/StdFileSystem.h"

namespace medialoader {

struct LoadOptions {
	bool triangulate = true;
	bool generateNormalsIfMissing = true;
	bool joinIdenticalVertices = true;
	bool flipUVs = true;
	bool improveCacheLocality = true;
};

class ModelLoader {
public:
	/** If fs is nullptr, an internal StdFileSystem is used. */
	explicit ModelLoader(FileSystem *fs = nullptr);

	ModelScene loadFromPath(const char *path, const LoadOptions &opt = LoadOptions());
	ModelScene loadFromMemory(const void *data, size_t size, const char *hintExt,
	                          const LoadOptions &opt = LoadOptions());

private:
	unsigned int flagsFrom(const LoadOptions &opt) const;

	StdFileSystem ownedFs_;
	FileSystem *fs_;
};

} // namespace medialoader
