// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/RenderElement/IRenderElement.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Scene {
class MeshNode;
} // namespace Stone::Scene

namespace Stone::Render::Vulkan {

class Device;
class RenderPass;

class MeshNode : public Scene::IRendererObject {
public:
	MeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode, const std::shared_ptr<Device> &device,
			 const std::shared_ptr<RenderPass> &renderPass, VkExtent2D extent);

	~MeshNode() override;

	void render(Scene::RenderContext &context) override;

private:
	void _createGraphicPipeline(const std::shared_ptr<RenderPass> &renderPass, VkExtent2D extent);
	void _destroyGraphicPipeline();

	std::shared_ptr<Device> _device;

	VkPipeline _graphicPipeline = VK_NULL_HANDLE;
	VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
};

} // namespace Stone::Render::Vulkan
