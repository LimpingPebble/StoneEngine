// Copyright 2024 Stone-Engine

#include "FramesRenderer.hpp"

#include "Device.hpp"

#include <iostream>


namespace Stone::Render::Vulkan {

FramesRenderer::FramesRenderer(const std::shared_ptr<Device> &device, uint32_t imageCount)
	: _device(device), _imageCount(imageCount) {
	std::cout << "Creating frames renderer" << std::endl;
	_createCommandBuffers();
	_createSyncObjects();
}

FramesRenderer::~FramesRenderer() {
	if (_device) {
		_device->waitIdle();
	}

	_destroySyncObjects();
	_destroyCommandBuffers();
	std::cout << "Destroying frames renderer" << std::endl;
}


/** Command Buffers */

void FramesRenderer::_createCommandBuffers() {

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

void FramesRenderer::_destroyCommandBuffers() {
	if (_commandBuffers.empty()) {
		return;
	}
	vkFreeCommandBuffers(_device->getDevice(), _device->getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()),
						 _commandBuffers.data());
	_commandBuffers.clear();
}


/** Sync Objects */

void FramesRenderer::_createSyncObjects() {
	_syncObjects.clear();
	_syncObjects.reserve(_commandBuffers.size());
	for (size_t i = 0; i < _commandBuffers.size(); ++i) {
		_syncObjects.emplace_back(_device->getDevice());
	}
}

void FramesRenderer::_destroySyncObjects() {
	_syncObjects.clear();
}

FramesRenderer::SyncronizedObjects::SyncronizedObjects(const VkDevice &device) : _device(device) {
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

FramesRenderer::SyncronizedObjects::~SyncronizedObjects() {
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

} // namespace Stone::Render::Vulkan
