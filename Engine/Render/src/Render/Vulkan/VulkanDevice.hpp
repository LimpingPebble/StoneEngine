// Copyright 2024 Stone-Engine

#pragma once

#include "Render/Vulkan/VulkanSettings.hpp"

namespace Stone::Render {

class VulkanDevice : std::enable_shared_from_this<VulkanDevice> {
public:
	VulkanDevice() = delete;
	explicit VulkanDevice(VulkanSettings &settings);
	VulkanDevice(const VulkanDevice &) = delete;

	virtual ~VulkanDevice();

	VkShaderModule createShaderModule(const std::vector<char> &code) const;

	VkDevice &getDevice();
	void waitIdle() const;

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

public: // TODO: Remove public
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

} // namespace Stone::Render
