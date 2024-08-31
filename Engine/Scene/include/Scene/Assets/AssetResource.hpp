// Copyrighted 2024 Stone-Engine

#pragma once

#include "Core/Assets/Resource.hpp"
#include "SceneTypes.hpp"

namespace Stone::Scene {

/**
 * @brief Represents a resource that contains meshes, textures, and other assets.
 */
class AssetResource : public Core::Assets::Resource {

public:

    AssetResource() = delete;
    AssetResource(const AssetResource &other) = delete;

    AssetResource(const std::shared_ptr<Core::Assets::Bundle>& bundle, const std::string &filepath);

    ~AssetResource() override = default;

    const char *getClassName() const override;

    std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

    const std::vector<std::shared_ptr<IMeshObject>>& getMeshes() const;
    std::vector<std::shared_ptr<IMeshObject>>& getMeshesRef();

    const std::vector<std::shared_ptr<Texture>>& getTextures() const;
    std::vector<std::shared_ptr<Texture>>& getTexturesRef();

    const std::vector<std::shared_ptr<Material>>& getMaterials() const;
    std::vector<std::shared_ptr<Material>>& getMaterialsRef();

    const std::shared_ptr<PivotNode>& getRootNode() const;

protected:

	std::vector<std::shared_ptr<IMeshObject>> _meshes;
	std::vector<std::shared_ptr<Texture>> _textures;
    std::vector<std::shared_ptr<Material>> _materials;
    std::shared_ptr<PivotNode> _rootNode;

    void loadData();
	void loadFromAssimp();
	void loadFromStone();

};


} // namespace Stone::Scene
