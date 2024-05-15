// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/RendererObjectManager.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

class Device;
class RenderPass;
class SwapChain;

class RendererObjectManager : public Scene::RendererObjectManager {

public:
	RendererObjectManager(const std::shared_ptr<Device> &device, const std::shared_ptr<RenderPass> &renderPass,
						  const std::shared_ptr<SwapChain> &swapChain);

	~RendererObjectManager() override = default;

	void updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) override;

	void updateTexture(const std::shared_ptr<Scene::Texture> &texture) override;

private:
	std::shared_ptr<Device> _device;
	std::shared_ptr<RenderPass> _renderPass;
	std::shared_ptr<SwapChain> _swapChain;
};

} // namespace Stone::Render::Vulkan
