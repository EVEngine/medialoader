#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <memory>

namespace medialoader {

/** RAII owner of Assimp::Importer (which owns the aiScene). */
class ModelScene {
public:
	ModelScene() = default;
	explicit ModelScene(std::unique_ptr<Assimp::Importer> importer);
	ModelScene(ModelScene &&) noexcept;
	ModelScene &operator=(ModelScene &&) noexcept;
	ModelScene(const ModelScene &) = delete;
	ModelScene &operator=(const ModelScene &) = delete;
	~ModelScene() = default;

	bool empty() const;
	const aiScene *get() const;
	const aiScene *operator->() const;
	const aiScene &operator*() const;

private:
	std::unique_ptr<Assimp::Importer> importer_;
};

} // namespace medialoader
