// Copyright 2024 Stone-Engine

#pragma once

#include "../Utilities/SwapChainProperties.hpp"
#include "Render/Vulkan/RendererSettings.hpp"

/**
 * @brief The VulkanDevice class represents a Vulkan device.
 *
 * This class provides functionality for interacting with a Vulkan device, such as creating shader modules,
 * managing buffers and images, and executing commands on the device.
 */
namespace Stone::Render::Vulkan {

class VulkanCore;

class VulkanDevice {
public:
	/**
	 * @brief Constructs a VulkanDevice object with the specified VulkanCore instance.
	 *
	 * @param core The VulkanCore instance associated with the device.
	 */
	explicit VulkanDevice(std::shared_ptr<VulkanCore> core);

	/**
	 * @brief Destructor for the VulkanDevice object.
	 */
	virtual ~VulkanDevice();

	/**
	 * @brief Waits for the device to become idle.
	 */
	void waitIdle() const;

	/**
	 * @brief Creates a Vulkan shader module from the provided shader code.
	 *
	 * @param code The shader code as a vector of characters.
	 * @return The created VkShaderModule object.
	 */
	[[nodiscard]] VkShaderModule createShaderModule(const std::vector<char> &code) const;

	/**
	 * @brief Creates swap chain properties for the specified size.
	 *
	 * @param size The size of the swap chain.
	 * @return The created SwapChainProperties object.
	 */
	[[nodiscard]] SwapChainProperties createSwapChainProperties(const std::pair<uint32_t, uint32_t> &size) const;

	/**
	 * @brief Finds a memory type that matches the specified filter and properties.
	 *
	 * @param typeFilter The memory type filter.
	 * @param properties The desired memory properties.
	 * @return The index of the matching memory type.
	 */
	[[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

	/**
	 * @brief Finds a supported format from the provided candidates.
	 *
	 * @param candidates The list of candidate formats.
	 * @param tiling The desired image tiling.
	 * @param features The desired format features.
	 * @return The supported VkFormat.
	 */
	[[nodiscard]] VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
											   VkFormatFeatureFlags features) const;

	/**
	 * @brief Finds a suitable depth format for the device.
	 *
	 * @return The suitable VkFormat for depth.
	 */
	[[nodiscard]] VkFormat findDepthFormat() const;

	/**
	 * @brief Executes the provided lambda function with a single Vulkan command buffer.
	 *
	 * @param lambda The lambda function to be executed with the command buffer.
	 */
	void withSingleCommandBuffer(const std::function<void(VkCommandBuffer)> &lambda) const;

	/**
	 * @brief Creates a Vulkan buffer with the specified size, usage, and memory properties.
	 *
	 * @param size The size of the buffer.
	 * @param usage The usage flags for the buffer.
	 * @param properties The memory properties for the buffer.
	 * @return A pair containing the created VkBuffer and VkDeviceMemory objects.
	 */
	std::pair<VkBuffer, VkDeviceMemory> createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
													 VkMemoryPropertyFlags properties) const;

	/**
	 * @brief Destroys the specified Vulkan buffer and associated device memory.
	 *
	 * @param buffer The VkBuffer to be destroyed.
	 * @param memory The VkDeviceMemory associated with the buffer.
	 */
	void destroyBuffer(VkBuffer buffer, VkDeviceMemory memory) const;

	/**
	 * @brief Copies data from one Vulkan buffer to another.
	 *
	 * @param dstBuffer The destination buffer to copy data to.
	 * @param srcBuffer The source buffer to copy data from.
	 * @param size The size of the data to be copied.
	 * @param commandBufferPtr A pointer to the command buffer to record the copy command into. If nullptr, a new
	 * command buffer will be allocated.
	 */
	void bufferCopy(VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize size,
					std::optional<VkCommandBuffer> commandBuffer = std::nullopt) const;

	/**
	 * @brief Creates a Vulkan image with the specified parameters.
	 *
	 * @param width The width of the image.
	 * @param height The height of the image.
	 * @param mipLevels The number of mip levels for the image.
	 * @param numSamples The number of samples for the image.
	 * @param format The format of the image.
	 * @param tiling The tiling mode for the image.
	 * @param usage The usage flags for the image.
	 * @param properties The memory properties for the image.
	 * @return A pair containing the created VkImage and VkDeviceMemory objects.
	 */
	std::pair<VkImage, VkDeviceMemory> createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
												   VkSampleCountFlagBits numSamples, VkFormat format,
												   VkImageTiling tiling, VkImageUsageFlags usage,
												   VkMemoryPropertyFlags properties) const;

	/**
	 * @brief Transitions the layout of a Vulkan image.
	 *
	 * @param image The VkImage to transition.
	 * @param format The format of the image.
	 * @param oldLayout The old layout of the image.
	 * @param newLayout The new layout of the image.
	 * @param commandBufferPtr A pointer to the command buffer to record the transition command into. If nullptr, a new
	 * command buffer will be allocated.
	 */
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
							   std::optional<VkCommandBuffer> commandBuffer = std::nullopt) const;

	/**
	 * @brief Copies data from a Vulkan buffer to an image.
	 *
	 * @param buffer The VkBuffer containing the data.
	 * @param image The VkImage to copy the data to.
	 * @param width The width of the image.
	 * @param height The height of the image.
	 * @param commandBufferPtr A pointer to the command buffer to record the copy command into. If nullptr, a new
	 * command buffer will be allocated.
	 */
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height,
						   std::optional<VkCommandBuffer> commandBuffer = std::nullopt) const;

	/**
	 * @brief Creates a VkImageView for the specified VkImage.
	 *
	 * @param image The VkImage to create the view for.
	 * @param format The format of the image.
	 * @param aspectFlags The aspect flags for the view. Default is VK_IMAGE_ASPECT_COLOR_BIT.
	 * @return The created VkImageView.
	 */
	VkImageView createImageView(VkImage image, VkFormat format,
								VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT) const;

	/**
	 * @brief Returns the VulkanCore instance associated with the device.
	 *
	 * @return The VulkanCore instance.
	 */
	const std::shared_ptr<VulkanCore> &getCore() const;


protected:
	/**
	 * @brief Returns the VkDevice object associated with the device.
	 *
	 * @return The VkDevice object.
	 */
	[[nodiscard]] const VkDevice &getVkDevice() const;

private:
	std::shared_ptr<VulkanCore> _core;
};

} // namespace Stone::Render::Vulkan
