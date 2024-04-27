// Copyright 2024 Stone-Engine

#pragma once

#include "SceneTypes.hpp"

#include <memory>

namespace Stone::Scene {

class ISceneRenderer {
public:
	/** Data Generation */
	virtual void generateDataForMesh(std::shared_ptr<Mesh> mesh) = 0;
	virtual void generateDataForSkinMesh(std::shared_ptr<SkinMesh> skinmesh) = 0;

	virtual void generateDataForMaterial(std::shared_ptr<Material> material) = 0;
	virtual void generateDataForTexture(std::shared_ptr<Texture> texture) = 0;

	/** Element Rendering */
	virtual void renderMeshNode(std::shared_ptr<MeshNode> node, RenderContext &context) = 0;
	virtual void renderInstancedMeshNode(std::shared_ptr<InstancedMeshNode> instancedmesh, RenderContext &context) = 0;
	virtual void renderSkinMeshNode(std::shared_ptr<SkinMeshNode> skinmesh, RenderContext &context) = 0;
};

} // namespace Stone::Scene
