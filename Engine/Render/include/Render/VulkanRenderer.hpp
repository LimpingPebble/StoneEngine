// Copyright 2024 Stone-Engine

#pragma once

#include "Render/Renderer.hpp"
#include "Scene.hpp"

namespace Stone::Render {

class VulkanRenderer : public Renderer {
public:
	VulkanRenderer();
	VulkanRenderer(const VulkanRenderer &) = delete;

	~VulkanRenderer() override;

	void generateDataForMesh(std::shared_ptr<Scene::Mesh> mesh) override;
	void generateDataForSkinMesh(std::shared_ptr<Scene::SkinMesh> skinmesh) override;

	void generateDataForMaterial(std::shared_ptr<Scene::Material> material) override;
	void generateDataForTexture(std::shared_ptr<Scene::Texture> texture) override;

	/** Element Rendering */
	void renderMeshNode(std::shared_ptr<Scene::MeshNode> node, Scene::RenderContext &context) override;
	void renderInstancedMeshNode(std::shared_ptr<Scene::InstancedMeshNode> instancedmesh, Scene::RenderContext &context) override;
	void renderSkinMeshNode(std::shared_ptr<Scene::SkinMeshNode> skinmesh, Scene::RenderContext &context) override;
};

} // namespace Stone::Render
