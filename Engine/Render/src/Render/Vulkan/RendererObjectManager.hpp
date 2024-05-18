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

	void updateTexture(const std::shared_ptr<Scene::Texture> &texture) override;

private:
	std::shared_ptr<VulkanRenderer> _renderer;
};

} // namespace Stone::Render::Vulkan
