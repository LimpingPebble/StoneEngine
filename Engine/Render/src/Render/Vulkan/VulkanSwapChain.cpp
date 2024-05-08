// Copyright 2024 Stone-Engine

#include "Render/Vulkan/VulkanSwapChain.hpp"

#include "Render/Vulkan/VulkanDevice.hpp"
#include "VulkanUtilities.hpp"

namespace Stone::Render {

VulkanSwapChain::VulkanSwapChain(const std::shared_ptr<VulkanDevice> &device, VulkanSwapChainProperties &props)
	: _device(device) {
	std::cout << "Creating swap chain" << std::endl;
	_createSwapChain(props);
	_createImageViews();

	_createRenderPass();
	_createFramebuffers();

	_createCommandBuffers();
	_createSyncObjects();
}

VulkanSwapChain::~VulkanSwapChain() {
	_destroySyncObjects();
	_destroyCommandBuffers();

	_destroyFramebuffers();
	_destroyRenderPass();

	_destroyImageViews();
	_destroySwapChain();
	std::cout << "Destroying swap chain" << std::endl;
}


/** Swap Chain */

void VulkanSwapChain::_createSwapChain(VulkanSwapChainProperties &props) {

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

	uint32_t imageCount = 0;
	vkGetSwapchainImagesKHR(_device->getDevice(), _swapChain, &imageCount, nullptr);
	_images.resize(imageCount);
	vkGetSwapchainImagesKHR(_device->getDevice(), _swapChain, &imageCount, _images.data());
	props.imageCount = imageCount;

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
	_imageViews.resize(_images.size());

	for (size_t i = 0; i < _images.size(); ++i) {
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

void VulkanSwapChain::_createFramebuffers() {
	_framebuffers.resize(_imageViews.size());

	for (size_t i = 0; i < _framebuffers.size(); ++i) {
		VkImageView attachments[] = {_imageViews[i]};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass;
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


/** Render Pass */

void VulkanSwapChain::_createRenderPass() {
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = _imageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(_device->getDevice(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create render pass");
	}
}

void VulkanSwapChain::_destroyRenderPass() {
	if (_renderPass == VK_NULL_HANDLE) {
		return;
	}
	vkDestroyRenderPass(_device->getDevice(), _renderPass, nullptr);
	_renderPass = VK_NULL_HANDLE;
}


/** Command Buffers */

void VulkanSwapChain::_createCommandBuffers() {

	_commandBuffers.resize(_framebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _device->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

	if (vkAllocateCommandBuffers(_device->getDevice(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffer");
	}
}

void VulkanSwapChain::_destroyCommandBuffers() {
	if (_commandBuffers.empty()) {
		return;
	}
	vkFreeCommandBuffers(_device->getDevice(), _device->getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()),
						 _commandBuffers.data());
	_commandBuffers.clear();
}


/** Sync Objects */

void VulkanSwapChain::_createSyncObjects() {
	_syncObjects.clear();
	_syncObjects.reserve(_commandBuffers.size());
	for (size_t i = 0; i < _commandBuffers.size(); ++i) {
		_syncObjects.emplace_back(_device->getDevice());
	}
}

void VulkanSwapChain::_destroySyncObjects() {
	_syncObjects.clear();
}

VulkanSwapChain::SyncronizedObjects::SyncronizedObjects(const VkDevice &device) : _device(device) {
	std::cout << "Creating syncronized objects" << std::endl;
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &imageAvailable) != VK_SUCCESS ||
		vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &renderFinished) != VK_SUCCESS ||
		vkCreateFence(_device, &fenceInfo, nullptr, &inFlight) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create synchronization objects for a frame");
	}
}

VulkanSwapChain::SyncronizedObjects::~SyncronizedObjects() {
	std::cout << "Destroying syncronized objects" << std::endl;
	if (imageAvailable != VK_NULL_HANDLE) {
		vkDestroySemaphore(_device, imageAvailable, nullptr);
	}
	if (renderFinished != VK_NULL_HANDLE) {
		vkDestroySemaphore(_device, renderFinished, nullptr);
	}
	if (inFlight != VK_NULL_HANDLE) {
		vkDestroyFence(_device, inFlight, nullptr);
	}
}

} // namespace Stone::Render
