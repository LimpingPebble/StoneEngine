// Copyright 2024 Stone-Engine

#include "Render/Vulkan/VulkanSwapChain.hpp"

#include "Render/Vulkan/VulkanDevice.hpp"
#include "VulkanUtilities.hpp"

namespace Stone::Render {

VulkanSwapChain::VulkanSwapChain(const std::shared_ptr<VulkanDevice> &device, const VkRenderPass &renderPass,
								 const VulkanSwapChainProperties &props)
	: _device(device) {
	std::cout << "Creating swap chain" << std::endl;
	_createSwapChain(props);
	_createImageViews();
	_createFramebuffers(renderPass);
}

VulkanSwapChain::~VulkanSwapChain() {
	if (_device) {
		_device->waitIdle();
	}

	_destroyFramebuffers();
	_destroyImageViews();
	_destroySwapChain();
	std::cout << "Destroying swap chain" << std::endl;
}


/** Swap Chain */

void VulkanSwapChain::_createSwapChain(const VulkanSwapChainProperties &props) {

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

void VulkanSwapChain::_destroySwapChain() {
	if (_swapChain == VK_NULL_HANDLE) {
		return;
	}
	vkDestroySwapchainKHR(_device->getDevice(), _swapChain, nullptr);
	_swapChain = VK_NULL_HANDLE;
}


/** Image Views */

void VulkanSwapChain::_createImageViews() {
	_imageViews.resize(_imageCount);

	for (size_t i = 0; i < _imageCount; ++i) {
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = _images[i];

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = _imageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(_device->getDevice(), &createInfo, nullptr, &_imageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image views");
		}
	}
}

void VulkanSwapChain::_destroyImageViews() {
	for (auto imageView : _imageViews) {
		vkDestroyImageView(_device->getDevice(), imageView, nullptr);
	}
	_imageViews.clear();
}


/** Framebuffers */

void VulkanSwapChain::_createFramebuffers(const VkRenderPass &renderPass) {
	_framebuffers.resize(_imageCount);

	for (size_t i = 0; i < _imageCount; ++i) {
		VkImageView attachments[] = {_imageViews[i]};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = _extent.width;
		framebufferInfo.height = _extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_device->getDevice(), &framebufferInfo, nullptr, &_framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create framebuffer");
		}
	}
}

void VulkanSwapChain::_destroyFramebuffers() {
	for (auto framebuffer : _framebuffers) {
		vkDestroyFramebuffer(_device->getDevice(), framebuffer, nullptr);
	}
	_framebuffers.clear();
}


} // namespace Stone::Render
