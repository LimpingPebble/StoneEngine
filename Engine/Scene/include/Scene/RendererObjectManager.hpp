// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/RenderElement/IRenderElement.hpp"
#include "SceneTypes.hpp"

namespace Stone::Scene {

class RendererObjectManager {
public:

	virtual ~RendererObjectManager() = default;

	virtual void updateMeshNode(const std::shared_ptr<MeshNode> &meshNode);
	virtual void updateInstancedMeshNode(const std::shared_ptr<InstancedMeshNode> &instancedMeshNode);
	virtual void updateSkinMeshNode(const std::shared_ptr<SkinMeshNode> &skinMeshNode);
	virtual void updateMaterial(const std::shared_ptr<Material> &material);
	virtual void updateMesh(const std::shared_ptr<Mesh> &mesh);
	virtual void updateSkinMesh(const std::shared_ptr<SkinMesh> &skinmesh);
	virtual void updateTexture(const std::shared_ptr<Texture> &texture);

protected:
	static void setRendererObjectTo(IRenderElement *element, const std::shared_ptr<IRendererObject> &rendererObject);
	static void markElementUndirty(IRenderElement *element);
};

} // namespace Stone::Scene
