// Copyright 2024 Stone-Engine

#pragma once

#include "Render/Vulkan/VulkanSettings.hpp"
#include "SwapChainProperties.hpp"

#include <optional>

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

	[[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

	/**
	 * Executes the provided lambda function with a single Vulkan command buffer.
	 *
	 * @param lambda The lambda function to be executed with the command buffer.
	 */
	void withSingleCommandBuffer(const std::function<void(VkCommandBuffer)> &lambda) const;

	std::pair<VkBuffer, VkDeviceMemory> createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
													 VkMemoryPropertyFlags properties) const;

	void destroyBuffer(VkBuffer buffer, VkDeviceMemory memory) const;

	/**
	 * Copies data from one Vulkan buffer to another.
	 *
	 * @param dstBuffer The destination buffer to copy data to.
	 * @param srcBuffer The source buffer to copy data from.
	 * @param size The size of the data to be copied.
	 * @param commandBufferPtr A pointer to the command buffer to record the copy command into. If nullptr, a new
	 * command buffer will be allocated.
	 */
	void bufferCopy(VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize size,
					std::optional<VkCommandBuffer> commandBuffer = std::nullopt) const;

	std::pair<VkImage, VkDeviceMemory> createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
												   VkSampleCountFlagBits numSamples, VkFormat format,
												   VkImageTiling tiling, VkImageUsageFlags usage,
												   VkMemoryPropertyFlags properties) const;

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
							   std::optional<VkCommandBuffer> commandBuffer = std::nullopt) const;

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height,
						   std::optional<VkCommandBuffer> commandBuffer = std::nullopt) const;

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
