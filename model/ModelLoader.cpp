#include "medialoader/model/ModelLoader.h"
#include "AssimpIO.h"

#include "medialoader/Exception.h"

#include <assimp/postprocess.h>

#include <memory>

namespace medialoader {

ModelLoader::ModelLoader(FileSystem *fs)
    : fs_(fs ? fs : &ownedFs_) {}

unsigned int ModelLoader::flagsFrom(const LoadOptions &opt) const {
	unsigned int flags = 0;
	if (opt.triangulate)
		flags |= aiProcess_Triangulate;
	if (opt.generateNormalsIfMissing)
		flags |= aiProcess_GenNormals;
	if (opt.joinIdenticalVertices)
		flags |= aiProcess_JoinIdenticalVertices;
	if (opt.flipUVs)
		flags |= aiProcess_FlipUVs;
	if (opt.improveCacheLocality)
		flags |= aiProcess_ImproveCacheLocality;
	return flags;
}

ModelScene ModelLoader::loadFromPath(const char *path, const LoadOptions &opt) {
	if (!path || !path[0])
		throw Exception("ModelLoader::loadFromPath: empty path");

	auto importer = std::unique_ptr<Assimp::Importer>(new Assimp::Importer());
	importer->SetIOHandler(new AssimpIOSystem(fs_));

	const aiScene *sc = importer->ReadFile(path, flagsFrom(opt));
	if (!sc || (sc->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !sc->mRootNode)
		throw Exception("ModelLoader::loadFromPath: %s", importer->GetErrorString());

	return ModelScene(std::move(importer));
}

ModelScene ModelLoader::loadFromMemory(const void *data, size_t size, const char *hintExt,
                                       const LoadOptions &opt) {
	if (!data || size == 0)
		throw Exception("ModelLoader::loadFromMemory: empty buffer");

	auto importer = std::unique_ptr<Assimp::Importer>(new Assimp::Importer());
	importer->SetIOHandler(new AssimpIOSystem(fs_));

	const char *hint = (hintExt && hintExt[0]) ? hintExt : "";
	const aiScene *sc =
	    importer->ReadFileFromMemory(data, size, flagsFrom(opt), hint);
	if (!sc || (sc->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !sc->mRootNode)
		throw Exception("ModelLoader::loadFromMemory: %s", importer->GetErrorString());

	return ModelScene(std::move(importer));
}

} // namespace medialoader
