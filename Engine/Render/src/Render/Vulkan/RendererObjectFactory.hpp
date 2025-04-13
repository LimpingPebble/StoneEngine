// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderer/RendererObjectFactory.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

class VulkanRenderer;

class RendererObjectFactory : public Scene::RendererObjectFactory {

public:
	RendererObjectFactory(const std::shared_ptr<VulkanRenderer> &renderer);
	RendererObjectFactory(const RendererObjectFactory &other) = default;
	~RendererObjectFactory() override = default;


	void updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) override;

	// void updateInstancedMeshNode(const std::shared_ptr<Scene::InstancedMeshNode> &instancedMeshNode) override;

	// void updateSkinMeshNode(const std::shared_ptr<Scene::SkinMeshNode> &skinMeshNode) override;

	void updateMaterial(const std::shared_ptr<Scene::Material> &material) override;

	void updateDynamicMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh) override;

	// void updateSkinMesh(const std::shared_ptr<Scene::SkinMesh> &skinmesh) override;

	void updateTexture(const std::shared_ptr<Scene::Texture> &texture) override;

	void updateFragmentShader(const std::shared_ptr<Scene::FragmentShader> &shader) override;

private:
	std::shared_ptr<VulkanRenderer> _renderer;
};

} // namespace Stone::Render::Vulkan
