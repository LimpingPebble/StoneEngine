// Copyright 2024 Stone-Engine

#include "Render/Vulkan/SwapChain.hpp"

#include "Render/Vulkan/Device.hpp"
#include "VulkanUtilities.hpp"

namespace Stone::Render::Vulkan {

SwapChain::SwapChain(const std::shared_ptr<Device> &device, const VkRenderPass &renderPass,
					 const SwapChainProperties &props)
	: _device(device) {
	std::cout << "Creating swap chain" << std::endl;
	_createSwapChain(props);
	_createImageViews();
	_createDepthResources();
	_createFramebuffers(renderPass);
}

SwapChain::~SwapChain() {
	if (_device) {
		_device->waitIdle();
	}

	_destroyFramebuffers();
	_destroyDepthResources();
	_destroyImageViews();
	_destroySwapChain();
	std::cout << "Destroying swap chain" << std::endl;
}

VkResult SwapChain::acquireNextImage(const VkSemaphore &semaphore, ImageContext &imageContext) {
	VkResult result = vkAcquireNextImageKHR(_device->getDevice(), _swapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE,
											&imageContext.index);
	imageContext.image = _images[imageContext.index];
	imageContext.imageView = _imageViews[imageContext.index];
	imageContext.framebuffer = _framebuffers[imageContext.index];
	return result;
}


/** Swap Chain */

void SwapChain::_createSwapChain(const SwapChainProperties &props) {

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = _device->getSurface();
	createInfo.minImageCount = props.minImageCount;
	createInfo.imageFormat = props.surfaceFormat.format;
	createInfo.imageColorSpace = props.surfaceFormat.colorSpace;
	createInfo.imageExtent = props.extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(_device->getPhysicalDevice(), _device->getSurface());
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = props.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = props.presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(_device->getDevice(), &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create swap chain");
	}

	_imageCount = 0;
	vkGetSwapchainImagesKHR(_device->getDevice(), _swapChain, &_imageCount, nullptr);
	_images.resize(_imageCount);
	vkGetSwapchainImagesKHR(_device->getDevice(), _swapChain, &_imageCount, _images.data());

	_imageFormat = props.surfaceFormat.format;
	_extent = props.extent;
}

void SwapChain::_destroySwapChain() {
	if (_swapChain == VK_NULL_HANDLE) {
		return;
	}
	vkDestroySwapchainKHR(_device->getDevice(), _swapChain, nullptr);
	_swapChain = VK_NULL_HANDLE;
}


/** Image Views */

void SwapChain::_createImageViews() {
	_imageViews.resize(_imageCount);

	for (size_t i = 0; i < _imageCount; ++i) {
		_imageViews[i] = _device->createImageView(_images[i], _imageFormat);
	}
}

void SwapChain::_destroyImageViews() {
	for (auto imageView : _imageViews) {
		vkDestroyImageView(_device->getDevice(), imageView, nullptr);
	}
	_imageViews.clear();
}


/** Depth Resources */

void SwapChain::_createDepthResources() {
	VkFormat depthFormat = _device->findDepthFormat();

	std::tie(_depthImage, _depthImageMemory) = _device->createImage(
		_extent.width, _extent.height, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	_depthImageView = _device->createImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	_device->transitionImageLayout(_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
								   VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void SwapChain::_destroyDepthResources() {
	vkDestroyImageView(_device->getDevice(), _depthImageView, nullptr);
	vkDestroyImage(_device->getDevice(), _depthImage, nullptr);
	vkFreeMemory(_device->getDevice(), _depthImageMemory, nullptr);
	_depthImageView = VK_NULL_HANDLE;
	_depthImage = VK_NULL_HANDLE;
	_depthImageMemory = VK_NULL_HANDLE;
}


/** Framebuffers */

void SwapChain::_createFramebuffers(const VkRenderPass &renderPass) {
	_framebuffers.resize(_imageCount);

	for (size_t i = 0; i < _imageCount; ++i) {

		std::array<VkImageView, 2> attachments = {_imageViews[i], _depthImageView};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = _extent.width;
		framebufferInfo.height = _extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_device->getDevice(), &framebufferInfo, nullptr, &_framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create framebuffer");
		}
	}
}

void SwapChain::_destroyFramebuffers() {
	for (auto framebuffer : _framebuffers) {
		vkDestroyFramebuffer(_device->getDevice(), framebuffer, nullptr);
	}
	_framebuffers.clear();
}


} // namespace Stone::Render::Vulkan
