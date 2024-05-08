// Copyright 2024 Stone-Engine

#pragma once

#include "VulkanSwapChainProperties.hpp"

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace Stone::Render {

class VulkanDevice;

class VulkanSwapChain {
public:
	VulkanSwapChain() = delete;
	VulkanSwapChain(const std::shared_ptr<VulkanDevice> &device, const VkRenderPass &renderPass,
					const VulkanSwapChainProperties &props);
	VulkanSwapChain(const VulkanSwapChain &) = delete;

	virtual ~VulkanSwapChain();

	[[nodiscard]] const VkFormat &getImageFormat() const {
		return _imageFormat;
	}

	[[nodiscard]] const VkExtent2D &getExtent() const {
		return _extent;
	}

	[[nodiscard]] uint32_t getImagecount() const {
		return _imageCount;
	}

private:
	void _createSwapChain(const VulkanSwapChainProperties &props);
	void _destroySwapChain();

	void _createImageViews();
	void _destroyImageViews();

	void _createFramebuffers(const VkRenderPass &renderPass);
	void _destroyFramebuffers();

	std::shared_ptr<VulkanDevice> _device;

	VkSwapchainKHR _swapChain = VK_NULL_HANDLE;
	uint32_t _imageCount = 0;
	std::vector<VkImage> _images = {};
	VkFormat _imageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D _extent = {0, 0};

	std::vector<VkImageView> _imageViews = {};

	std::vector<VkFramebuffer> _framebuffers = {};
};

} // namespace Stone::Render
