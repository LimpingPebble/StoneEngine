// Copyright 2024 Stone-Engine

#pragma once

#include "Render/Renderer.hpp"
#include "Render/Vulkan/VulkanSettings.hpp"

namespace Stone::Scene {
class WorldNode;
}

namespace Stone::Render::Vulkan {

class Device;
class RenderPass;
class FramesRenderer;
class SwapChain;
struct ImageContext;

class VulkanRenderer : public Renderer {
public:
	VulkanRenderer() = delete;
	explicit VulkanRenderer(VulkanSettings &settings);
	VulkanRenderer(const VulkanRenderer &) = delete;

	~VulkanRenderer() override;

	/** Renderer */

	void updateDataForWorld(const std::shared_ptr<Scene::WorldNode> &world) override;
	void renderWorld(const std::shared_ptr<Scene::WorldNode> &world) override;

	void updateFrameSize(std::pair<uint32_t, uint32_t> size) override;


private:
	void _recreateSwapChain(std::pair<uint32_t, uint32_t> size);

	void _recordCommandBuffer(VkCommandBuffer commandBuffer, ImageContext *imageContext,
							  const std::shared_ptr<Scene::WorldNode> &world);

	std::shared_ptr<Device> _device;
	std::shared_ptr<RenderPass> _renderPass;
	std::shared_ptr<FramesRenderer> _framesRenderer;
	std::shared_ptr<SwapChain> _swapChain;
};

} // namespace Stone::Render::Vulkan
