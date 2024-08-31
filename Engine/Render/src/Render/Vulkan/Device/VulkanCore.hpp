// Copyright 2024 Stone-Engine

#pragma once

#include "../Utilities/SwapChainProperties.hpp"
#include "Render/Vulkan/RendererSettings.hpp"

namespace Stone::Render::Vulkan {

class VulkanCore {
public:
	VulkanCore() = delete;
	explicit VulkanCore(RendererSettings &settings);
	VulkanCore(const VulkanCore &) = delete;

	virtual ~VulkanCore();

	const VkDevice &getDevice() {
		return _device;
	}

	const VkPhysicalDevice &getPhysicalDevice() {
		return _physicalDevice;
	}

	const VkSurfaceKHR &getSurface() {
		return _surface;
	}

	const VkQueue &getGraphicsQueue() {
		return _graphicsQueue;
	}

	const VkQueue &getPresentQueue() {
		return _presentQueue;
	}

	const VkCommandPool &getCommandPool() {
		return _commandPool;
	}

	void waitIdle() const;


private:
	void _createInstance(RendererSettings &settings);
	void _destroyInstance();

	void _setupDebugMessenger();
	void _destroyDebugMessenger();

	void _createSurface(RendererSettings &settings);
	void _destroySurface();

	void _pickPhysicalDevice(RendererSettings &settings);

	void _createLogicalDevice(RendererSettings &settings);
	void _destroyLogicalDevice();

	void _createCommandPool();
	void _destroyCommandPool();

	VkInstance _instance = VK_NULL_HANDLE;
#ifdef VALIDATION_LAYERS
	VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
#endif
	VkSurfaceKHR _surface = VK_NULL_HANDLE;
	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	VkDevice _device = VK_NULL_HANDLE;
	VkQueue _graphicsQueue = VK_NULL_HANDLE;
	VkQueue _presentQueue = VK_NULL_HANDLE;
	VkCommandPool _commandPool = VK_NULL_HANDLE;
};

} // namespace Stone::Render::Vulkan
