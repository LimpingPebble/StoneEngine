// Copyright 2024 Stone-Engine

#include "VulkanFramesRenderer.hpp"

#include "VulkanDevice.hpp"

#include <iostream>


namespace Stone::Render {

VulkanFramesRenderer::VulkanFramesRenderer(const std::shared_ptr<VulkanDevice> &device, uint32_t imageCount)
	: _device(device), _imageCount(imageCount) {
	std::cout << "Creating frames renderer" << std::endl;
	_createCommandBuffers();
	_createSyncObjects();
}

VulkanFramesRenderer::~VulkanFramesRenderer() {
	if (_device) {
		_device->waitIdle();
	}

	_destroySyncObjects();
	_destroyCommandBuffers();
	std::cout << "Destroying frames renderer" << std::endl;
}


/** Command Buffers */

void VulkanFramesRenderer::_createCommandBuffers() {

	_commandBuffers.resize(_imageCount);

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _device->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

	if (vkAllocateCommandBuffers(_device->getDevice(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffer");
	}
}

void VulkanFramesRenderer::_destroyCommandBuffers() {
	if (_commandBuffers.empty()) {
		return;
	}
	vkFreeCommandBuffers(_device->getDevice(), _device->getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()),
						 _commandBuffers.data());
	_commandBuffers.clear();
}


/** Sync Objects */

void VulkanFramesRenderer::_createSyncObjects() {
	_syncObjects.clear();
	_syncObjects.reserve(_commandBuffers.size());
	for (size_t i = 0; i < _commandBuffers.size(); ++i) {
		_syncObjects.emplace_back(_device->getDevice());
	}
}

void VulkanFramesRenderer::_destroySyncObjects() {
	_syncObjects.clear();
}

VulkanFramesRenderer::SyncronizedObjects::SyncronizedObjects(const VkDevice &device) : _device(device) {
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

VulkanFramesRenderer::SyncronizedObjects::~SyncronizedObjects() {
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
