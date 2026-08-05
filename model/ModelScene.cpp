#include "medialoader/model/ModelScene.h"

#include <utility>

namespace medialoader {

ModelScene::ModelScene(std::unique_ptr<Assimp::Importer> importer)
    : importer_(std::move(importer)) {}

ModelScene::ModelScene(ModelScene &&other) noexcept
    : importer_(std::move(other.importer_)) {}

ModelScene &ModelScene::operator=(ModelScene &&other) noexcept {
	if (this != &other)
		importer_ = std::move(other.importer_);
	return *this;
}

bool ModelScene::empty() const {
	return !importer_ || importer_->GetScene() == nullptr;
}

const aiScene *ModelScene::get() const {
	return importer_ ? importer_->GetScene() : nullptr;
}

const aiScene *ModelScene::operator->() const {
	return get();
}

const aiScene &ModelScene::operator*() const {
	return *get();
}

} // namespace medialoader
