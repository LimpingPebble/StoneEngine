// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/RendererObjectManager.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

class VulkanRenderer;

class RendererObjectManager : public Scene::RendererObjectManager {

public:
	RendererObjectManager(const std::shared_ptr<VulkanRenderer> &renderer);

	~RendererObjectManager() override = default;


	void updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) override;

	// void updateInstancedMeshNode(const std::shared_ptr<Scene::InstancedMeshNode> &instancedMeshNode) override;

	// void updateSkinMeshNode(const std::shared_ptr<Scene::SkinMeshNode> &skinMeshNode) override;

	void updateMaterial(const std::shared_ptr<Scene::Material> &material) override;

	void updateMesh(const std::shared_ptr<Scene::Mesh> &mesh) override;

	// void updateSkinMesh(const std::shared_ptr<Scene::SkinMesh> &skinmesh) override;

	void updateTexture(const std::shared_ptr<Scene::Texture> &texture) override;

	void updateShader(const std::shared_ptr<Scene::Shader> &shader) override;

private:
	std::shared_ptr<VulkanRenderer> _renderer;
};

} // namespace Stone::Render::Vulkan
