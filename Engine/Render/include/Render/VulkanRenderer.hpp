// Copyright 2024 Stone-Engine

#pragma once

#include "Render/Renderer.hpp"
#include "Render/VulkanSettings.hpp"
#include "Scene.hpp"

namespace Stone::Render {

class VulkanDevice;

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
	// void _createSwapChain(const std::pair<uint32_t, uint32_t> &size);
	// void _destroySwapChain();

	// void _createImageViews();
	// void _destroyImageViews();

	// void _createRenderPass();
	// void _destroyRenderPass();

	// void _createGraphicPipeline();
	// void _destroyGraphicPipeline();

	// void _createFramebuffers();
	// void _destroyFramebuffers();

	// void _recreateSwapChain(std::pair<uint32_t, uint32_t> size);

	// void _createCommandBuffers();
	// void _destroyCommandBuffers();

	// void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	// void _createSyncObjects();
	// void _destroySyncObjects();

	std::shared_ptr<VulkanDevice> _device;

	// VkSwapchainKHR _swapChain = VK_NULL_HANDLE;
	// std::vector<VkImage> _swapChainImages = {};
	// VkFormat _swapChainImageFormat = VK_FORMAT_UNDEFINED;
	// VkExtent2D _swapChainExtent = {0, 0};
	// std::vector<VkImageView> _swapChainImageViews = {};
	// VkRenderPass _renderPass = VK_NULL_HANDLE;
	// VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
	// VkPipeline _graphicsPipeline = VK_NULL_HANDLE;
	// std::vector<VkFramebuffer> _swapChainFramebuffers = {};
	// VkCommandPool _commandPool = VK_NULL_HANDLE;

	// std::vector<VkCommandBuffer> _commandBuffers = {};

	// struct SyncronizedObjects {
	// 	VkSemaphore imageAvailable = VK_NULL_HANDLE;
	// 	VkSemaphore renderFinished = VK_NULL_HANDLE;
	// 	VkFence inFlight = VK_NULL_HANDLE;
	// 	VkDevice &_device;

	// 	explicit SyncronizedObjects(VkDevice &device);
	// 	~SyncronizedObjects();
	// };

	// std::vector<SyncronizedObjects> _syncObjects;
	// size_t _currentFrame = 0;
};

} // namespace Stone::Render
