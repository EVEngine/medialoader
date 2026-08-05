#include "medialoader/Exception.h"
#include "medialoader/model/ModelLoader.h"
#include "medialoader/model/StdFileSystem.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <cstdio>
#include <cstring>
#include <string>

#define CHECK(cond)                                                                              \
	do {                                                                                         \
		if (!(cond)) {                                                                           \
			std::fprintf(stderr, "CHECK failed: %s (%s:%d)\n", #cond, __FILE__, __LINE__);       \
			return 1;                                                                            \
		}                                                                                        \
	} while (0)

// Minimal unit cube as Wavefront OBJ.
static const char kCubeObj[] =
    "v -0.5 -0.5 -0.5\n"
    "v  0.5 -0.5 -0.5\n"
    "v  0.5  0.5 -0.5\n"
    "v -0.5  0.5 -0.5\n"
    "v -0.5 -0.5  0.5\n"
    "v  0.5 -0.5  0.5\n"
    "v  0.5  0.5  0.5\n"
    "v -0.5  0.5  0.5\n"
    "f 1 2 3\n"
    "f 1 3 4\n"
    "f 5 6 7\n"
    "f 5 7 8\n"
    "f 1 2 6\n"
    "f 1 6 5\n"
    "f 2 3 7\n"
    "f 2 7 6\n"
    "f 3 4 8\n"
    "f 3 8 7\n"
    "f 4 1 5\n"
    "f 4 5 8\n";

static int test_std_filesystem_roundtrip() {
	const char *path = "ml_fs_test_tmp.bin";
	FILE *w = std::fopen(path, "wb");
	CHECK(w);
	const char payload[] = "hello";
	CHECK(std::fwrite(payload, 1, 5, w) == 5);
	std::fclose(w);

	medialoader::StdFileSystem fs;
	CHECK(fs.exists(path));
	medialoader::FileHandle *h = fs.open(path, "rb");
	CHECK(h);
	CHECK(fs.size(h) == 5);
	char buf[8] = {};
	CHECK(fs.read(h, buf, 5) == 5);
	CHECK(std::string(buf, 5) == "hello");
	fs.close(h);
	std::remove(path);
	return 0;
}

static int test_load_cube_from_memory() {
	medialoader::ModelLoader loader;
	medialoader::ModelScene scene =
	    loader.loadFromMemory(kCubeObj, sizeof(kCubeObj) - 1, ".obj");
	CHECK(!scene.empty());
	CHECK(scene->mNumMeshes >= 1u);
	CHECK(scene->mMeshes[0]->mNumFaces >= 1u);
	CHECK(scene->mMeshes[0]->mFaces[0].mNumIndices == 3u);
	return 0;
}

static int test_load_cube_from_path() {
	const char *path = "ml_cube_tmp.obj";
	FILE *f = std::fopen(path, "wb");
	CHECK(f);
	CHECK(std::fwrite(kCubeObj, 1, sizeof(kCubeObj) - 1, f) == sizeof(kCubeObj) - 1);
	std::fclose(f);

	medialoader::ModelLoader loader;
	medialoader::ModelScene scene = loader.loadFromPath(path);
	CHECK(!scene.empty());
	CHECK(scene->mNumMeshes >= 1u);
	std::remove(path);
	return 0;
}

static int test_load_missing_throws() {
	medialoader::ModelLoader loader;
	bool threw = false;
	try {
		loader.loadFromPath("definitely_missing_ml_model.obj");
	} catch (const medialoader::Exception &) {
		threw = true;
	}
	CHECK(threw);
	return 0;
}

int main() {
	if (test_std_filesystem_roundtrip())
		return 1;
	if (test_load_cube_from_memory())
		return 1;
	if (test_load_cube_from_path())
		return 1;
	if (test_load_missing_throws())
		return 1;
	std::printf("medialoader_model_test: all passed\n");
	return 0;
}
