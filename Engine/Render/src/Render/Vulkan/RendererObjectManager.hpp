// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/RendererObjectManager.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

class Device;
class RenderPass;

class RendererObjectManager : public Scene::RendererObjectManager {

public:
	RendererObjectManager(const std::shared_ptr<Device> &device, const std::shared_ptr<RenderPass> &renderPass,
						  VkExtent2D extent);

	~RendererObjectManager() override = default;

	void updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) override;

private:
	std::shared_ptr<Device> _device;
	std::shared_ptr<RenderPass> _renderPass;
	VkExtent2D _extent;
};

} // namespace Stone::Render::Vulkan
