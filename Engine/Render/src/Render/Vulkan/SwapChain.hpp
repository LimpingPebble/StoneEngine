// Copyright 2024 Stone-Engine

#pragma once

#include "SwapChainProperties.hpp"

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

class Device;

struct ImageContext {
	uint32_t index;
	VkImage image;
	VkImageView imageView;
	VkFramebuffer framebuffer;
};

class SwapChain {
public:
	SwapChain() = delete;
	SwapChain(const std::shared_ptr<Device> &device, const VkRenderPass &renderPass, const SwapChainProperties &props);
	SwapChain(const SwapChain &) = delete;

	virtual ~SwapChain();

	[[nodiscard]] const VkSwapchainKHR &getSwapChain() const {
		return _swapChain;
	}

	[[nodiscard]] const VkFormat &getImageFormat() const {
		return _imageFormat;
	}

	[[nodiscard]] const VkExtent2D &getExtent() const {
		return _extent;
	}

	[[nodiscard]] uint32_t getImagecount() const {
		return _imageCount;
	}

	VkResult acquireNextImage(const VkSemaphore &semaphore, ImageContext &imageContext);

private:
	void _createSwapChain(const SwapChainProperties &props);
	void _destroySwapChain();

	void _createImageViews();
	void _destroyImageViews();

	void _createFramebuffers(const VkRenderPass &renderPass);
	void _destroyFramebuffers();

	std::shared_ptr<Device> _device;

	VkSwapchainKHR _swapChain = VK_NULL_HANDLE;
	uint32_t _imageCount = 0;
	std::vector<VkImage> _images = {};
	VkFormat _imageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D _extent = {0, 0};

	std::vector<VkImageView> _imageViews = {};

	std::vector<VkFramebuffer> _framebuffers = {};
};

} // namespace Stone::Render::Vulkan
