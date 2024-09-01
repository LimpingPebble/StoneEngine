// Copyright 2024 Stone-Engine

#pragma once

#include "Utilities/SwapChainProperties.hpp"

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

class Device;

struct SyncronizedObjects {
	VkSemaphore imageAvailable = VK_NULL_HANDLE;
	VkSemaphore renderFinished = VK_NULL_HANDLE;
	VkFence inFlight = VK_NULL_HANDLE;
	const VkDevice &_device;

	explicit SyncronizedObjects(const VkDevice &device);
	~SyncronizedObjects();
};

struct FrameContext {
	VkCommandBuffer &commandBuffer;
	SyncronizedObjects &syncObject;
};

class FramesRenderer {
public:
	FramesRenderer() = delete;
	FramesRenderer(const std::shared_ptr<Device> &device, uint32_t imageCount);
	FramesRenderer(const FramesRenderer &) = delete;

	virtual ~FramesRenderer();

	[[nodiscard]] uint32_t getImageCount() const {
		return _imageCount;
	}

	FrameContext newFrameContext();

private:
	void _createCommandBuffers();
	void _destroyCommandBuffers();

	void _createSyncObjects();
	void _destroySyncObjects();

	std::shared_ptr<Device> _device;
	uint32_t _imageCount;

	std::vector<VkCommandBuffer> _commandBuffers = {};

	std::vector<SyncronizedObjects> _syncObjects = {};
	size_t _currentFrame = 0;
};

} // namespace Stone::Render::Vulkan
