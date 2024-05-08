// Copyright 2024 Stone-Engine

#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace Stone::Render {

class VulkanDevice;

class VulkanSwapChain {
public:
	VulkanSwapChain() = delete;
	VulkanSwapChain(const std::shared_ptr<VulkanDevice> &device, const std::pair<uint32_t, uint32_t> &size);
	VulkanSwapChain(const VulkanSwapChain &) = delete;

	virtual ~VulkanSwapChain();

	[[nodiscard]] const VkFormat &getImageFormat() const {
		return _imageFormat;
	}

	[[nodiscard]] const VkExtent2D &getExtent() const {
		return _extent;
	}

private:
	void _createSwapChain(const std::pair<uint32_t, uint32_t> &size);
	void _destroySwapChain();

	void _createImageViews();
	void _destroyImageViews();

	void _createFramebuffers();
	void _destroyFramebuffers();

	void _createRenderPass();
	void _destroyRenderPass();

	void _createCommandBuffers();
	void _destroyCommandBuffers();

	void _createSyncObjects();
	void _destroySyncObjects();

	std::shared_ptr<VulkanDevice> _device;

	VkSwapchainKHR _swapChain = VK_NULL_HANDLE;
	std::vector<VkImage> _images = {};
	VkFormat _imageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D _extent = {0, 0};

	std::vector<VkImageView> _imageViews = {};

	std::vector<VkFramebuffer> _framebuffers = {};

	VkRenderPass _renderPass = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> _commandBuffers = {};

	struct SyncronizedObjects {
		VkSemaphore imageAvailable = VK_NULL_HANDLE;
		VkSemaphore renderFinished = VK_NULL_HANDLE;
		VkFence inFlight = VK_NULL_HANDLE;
		const VkDevice &_device;

		explicit SyncronizedObjects(const VkDevice &device);
		~SyncronizedObjects();
	};

	std::vector<SyncronizedObjects> _syncObjects;
	size_t _currentFrame = 0;
};

} // namespace Stone::Render
