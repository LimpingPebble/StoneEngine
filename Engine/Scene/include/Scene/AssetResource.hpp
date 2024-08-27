// Copyrighted 2024 Stone-Engine

#pragma once

#include "Core/Image/ImageAlbum.hpp"
#include "SceneTypes.hpp"

#include <map>
#include <vector>

namespace Stone::Scene {

/**
 * @struct AssetResource
 * @brief Represents a resource that contains meshes, textures, and other assets.
 */
struct AssetResource {

	AssetResource(const std::string &filepath = "", const std::shared_ptr<Image::Album> &imagesAlbum = nullptr);

	/**
	 * @brief The filename of the asset without it's path
	 */
	std::string filename;

	/**
	 * @brief The directory of the asset
	 */
	std::string directory;

	/**
	 * @brief An image album that contains all the images of the asset
	 */
	std::shared_ptr<Image::Album> imagesAlbum = nullptr;

	/**
	 * @brief The meshes of the asset
	 */
	std::vector<std::shared_ptr<IMeshObject>> meshes;
	std::vector<std::shared_ptr<Texture>> textures;
    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<PivotNode> rootNode;
};

} // namespace Stone::Scene
