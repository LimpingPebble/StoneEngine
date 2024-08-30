// Copyrighted 2024 Stone-Engine

#include "Scene/AssetsManager.hpp"

namespace Stone::Scene {

AssetResource::AssetResource(const std::string &filepath, const std::shared_ptr<Image::Album> &imagesAlbum)
	: filename(filepath), directory(), imagesAlbum(imagesAlbum) {

	auto slashPos = filepath.find_last_of('/');
	if (slashPos != std::string::npos) {
		directory = filepath.substr(0, slashPos);
		filename = filepath.substr(slashPos + 1);
	}

	if (imagesAlbum == nullptr) {
		this->imagesAlbum = std::make_shared<Image::Album>();
	}
};

std::shared_ptr<AssetResource> AssetResource::load(const std::string &filepath, const std::shared_ptr<Image::Album> &imagesAlbum)
{
	if (filepath.ends_with(".stone")) {
		return loadStone(filepath, imagesAlbum);
	} else {
		return loadAssimp(filepath, imagesAlbum);
	}
}

} // namespace Stone::Scene
