// Copyright 2024 Stone-Engine

#pragma once

#include "Render/Vulkan/VulkanSettings.hpp"
#include "SwapChainProperties.hpp"

namespace Stone::Render::Vulkan {

class Device {
public:
	Device() = delete;
	explicit Device(VulkanSettings &settings);
	Device(const Device &) = delete;

	virtual ~Device();

	[[nodiscard]] VkShaderModule createShaderModule(const std::vector<char> &code) const;

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

	[[nodiscard]] SwapChainProperties createSwapChainProperties(const std::pair<uint32_t, uint32_t> &size) const;

private:
	void _createInstance(VulkanSettings &settings);
	void _destroyInstance();

	void _setupDebugMessenger();
	void _destroyDebugMessenger();

	void _createSurface(VulkanSettings &settings);
	void _destroySurface();

	void _pickPhysicalDevice(VulkanSettings &settings);

	void _createLogicalDevice(VulkanSettings &settings);
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
