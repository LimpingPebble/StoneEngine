// Copyright 2024 Stone-Engine

#include "Render/Vulkan/Device.hpp"

#include "VulkanUtilities.hpp"

#include <iostream>
#include <set>

namespace Stone::Render::Vulkan {

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
											 VkDebugUtilsMessageTypeFlagsEXT messageType,
											 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
											 void *pUserData) {
	(void)pUserData;
	// TODO: Log message with logging module
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		std::cerr << "ValidationLayer[" << to_string(messageSeverity) << "][" << to_string(messageType)
				  << "]:" << pCallbackData->pMessage << std::endl;
	}

	return VK_FALSE;
}

Device::Device(VulkanSettings &settings) {
	std::cout << "Device created" << std::endl;
	_createInstance(settings);
	_setupDebugMessenger();
	_createSurface(settings);
	_pickPhysicalDevice(settings);
	_createLogicalDevice(settings);
	_createCommandPool();
}

Device::~Device() {
	waitIdle();

	_destroyCommandPool();
	_destroyLogicalDevice();
	_destroySurface();
	_destroyDebugMessenger();
	_destroyInstance();
	std::cout << "Device destroyed" << std::endl;
}

VkShaderModule Device::createShaderModule(const std::vector<char> &code) const {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create shader module");
	}

	return shaderModule;
}

void Device::waitIdle() const {
	vkDeviceWaitIdle(_device);
}

SwapChainProperties Device::createSwapChainProperties(const std::pair<uint32_t, uint32_t> &size) const {
	SwapChainProperties settings;

	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(_physicalDevice, _surface);

	settings.surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	settings.presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

	settings.capabilities = swapChainSupport.capabilities;

	auto [width, height] = size;
	settings.extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}
	settings.minImageCount = imageCount;
	settings.imageCount = imageCount;

	return settings;
}


uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type");
}

VkFormat Device::findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
									 VkFormatFeatureFlags features) const {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}

		if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("Failed to find supported format");
}

VkFormat Device::findDepthFormat() const {
	return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
							   VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

void Device::withSingleCommandBuffer(const std::function<void(VkCommandBuffer)> &lambda) const {
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	lambda(commandBuffer);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(_graphicsQueue);

	vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
}

std::pair<VkBuffer, VkDeviceMemory> Device::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
														 VkMemoryPropertyFlags properties) const {
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.flags = 0;

	if (vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create buffer");
	}

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(_device, buffer, &memoryRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate buffer memory");
	}

	vkBindBufferMemory(_device, buffer, bufferMemory, 0);

	return {buffer, bufferMemory};
}

void Device::destroyBuffer(VkBuffer buffer, VkDeviceMemory memory) const {
	if (buffer != VK_NULL_HANDLE) {
		vkDestroyBuffer(_device, buffer, nullptr);
	}
	if (memory != VK_NULL_HANDLE) {
		vkFreeMemory(_device, memory, nullptr);
	}
}

void Device::bufferCopy(VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize size,
						std::optional<VkCommandBuffer> commandBuffer) const {

	auto lambda = [&](VkCommandBuffer cmdBuff) {
		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;

		vkCmdCopyBuffer(cmdBuff, srcBuffer, dstBuffer, 1, &copyRegion);
	};

	if (commandBuffer) {
		lambda(*commandBuffer);
	} else {
		withSingleCommandBuffer(lambda);
	}
}

std::pair<VkImage, VkDeviceMemory> Device::createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
													   VkSampleCountFlagBits numSamples, VkFormat format,
													   VkImageTiling tiling, VkImageUsageFlags usage,
													   VkMemoryPropertyFlags properties) const {
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = numSamples;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkImage image;
	if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create image");
	}

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(_device, image, &memoryRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, properties);

	VkDeviceMemory imageMemory;
	if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate image memory");
	}

	vkBindImageMemory(_device, image, imageMemory, 0);

	return {image, imageMemory};
}

void Device::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
								   std::optional<VkCommandBuffer> commandBuffer) const {
	(void)format;
	auto lambda = [&](VkCommandBuffer cmdBuff) {
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (hasStencilComponent(format)) {
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		} else {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;


		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
				   newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
				   newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask =
				VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		} else {
			throw std::invalid_argument("Unsupported layout transition");
		}

		vkCmdPipelineBarrier(cmdBuff, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	};

	if (commandBuffer) {
		lambda(*commandBuffer);
	} else {
		withSingleCommandBuffer(lambda);
	}
}


void Device::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height,
							   std::optional<VkCommandBuffer> commandBuffer) const {
	auto lambda = [&](VkCommandBuffer cmdBuff) {
		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = {0, 0, 0};
		region.imageExtent = {width, height, 1};

		vkCmdCopyBufferToImage(cmdBuff, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	};

	if (commandBuffer) {
		lambda(*commandBuffer);
	} else {
		withSingleCommandBuffer(lambda);
	}
}

VkImageView Device::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const {
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create texture image view");
	}

	return imageView;
}


/** Instance */

void Device::_createInstance(VulkanSettings &settings) {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = settings.app_name.c_str();
	appInfo.applicationVersion = settings.app_version;
	appInfo.pEngineName = "Stone-Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

#ifdef VALIDATION_LAYERS
	if (!checkValidationLayerSupport(settings.validationLayers)) {
		throw std::runtime_error("Validation layers requested, but not available!");
	}
	createInfo.enabledLayerCount = static_cast<uint32_t>(settings.validationLayers.size());
	createInfo.ppEnabledLayerNames = settings.validationLayers.data();
	settings.instanceExt.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#else
	createInfo.enabledLayerCount = 0;
#endif

	settings.instanceExt.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	createInfo.enabledExtensionCount = static_cast<uint32_t>(settings.instanceExt.size());
	createInfo.ppEnabledExtensionNames = settings.instanceExt.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Vulkan instance");
	}

	// enumerateExtensions(std::cout);
}

void Device::_destroyInstance() {
	if (_instance == VK_NULL_HANDLE) {
		return;
	}
	vkDestroyInstance(_instance, nullptr);
	_instance = VK_NULL_HANDLE;
}

/** Debug Messenger */

void Device::_setupDebugMessenger() {
#ifdef VALIDATION_LAYERS
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = &debugCallback;
	createInfo.pUserData = nullptr;

	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(_instance, &createInfo, nullptr, &_debugMessenger);
	} else {
		throw std::runtime_error("Failed to setup debug messenger");
	}
#endif
}

void Device::_destroyDebugMessenger() {
#ifdef VALIDATION_LAYERS
	if (_debugMessenger == VK_NULL_HANDLE) {
		return;
	}
	auto func =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(_instance, _debugMessenger, nullptr);
	}
	_debugMessenger = VK_NULL_HANDLE;
#endif
}

/** Surface */

void Device::_createSurface(VulkanSettings &settings) {
	if (settings.createSurface != nullptr) {
		if (settings.createSurface(_instance, nullptr, &_surface) == VK_SUCCESS) {
			return;
		}
	}
	throw std::runtime_error("Failed to create window surface !");
}

void Device::_destroySurface() {
	if (_surface == VK_NULL_HANDLE) {
		return;
	}
	vkDestroySurfaceKHR(_instance, _surface, nullptr);
	_surface = VK_NULL_HANDLE;
}

/** Physical Device */

bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char *> &deviceExtensions) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto &extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}
	return requiredExtensions.empty();
}

int deviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface,
					  const std::vector<const char *> &deviceExtensions) {
	if (device == VK_NULL_HANDLE) {
		return -1;
	}

	int score = 0;
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);

	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);
#ifndef __APPLE__
	if (!features.geometryShader) {
		return -1;
	}
#endif

	QueueFamilyIndices indices = findQueueFamilies(device, surface);
	if (!indices.isComplete()) {
		return -1;
	}

	if (!checkDeviceExtensionSupport(device, deviceExtensions)) {
		return -1;
	}

	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
	if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) {
		return -1;
	}

	score += static_cast<int>(properties.limits.maxImageDimension2D);

	return score;
}

void Device::_pickPhysicalDevice(VulkanSettings &settings) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

	int bestDeviceIndex = -1;
	int bestScore = -1;
	for (uint32_t i = 0; i < deviceCount; i++) {
		int score = deviceSuitability(devices[i], _surface, settings.deviceExt);
		if (score > bestScore) {
			bestScore = score;
			bestDeviceIndex = static_cast<int>(i);
		}
	}
	if (bestDeviceIndex < 0 || bestScore < 0) {
		throw std::runtime_error("Failed to find a suitable GPU");
	}
	_physicalDevice = devices[bestDeviceIndex];
}

/** Logical device */

void Device::_createLogicalDevice(VulkanSettings &settings) {
	QueueFamilyIndices indices = findQueueFamilies(_physicalDevice, _surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.emplace_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = settings.deviceExt.size();
	createInfo.ppEnabledExtensionNames = settings.deviceExt.data();

#ifdef VALIDATION_LAYERS
	createInfo.enabledLayerCount = static_cast<uint32_t>(settings.validationLayers.size());
	createInfo.ppEnabledLayerNames = settings.validationLayers.data();
#else
	createInfo.enabledLayerCount = 0;
#endif

	if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device");
	}

	vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
	vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
}

void Device::_destroyLogicalDevice() {
	if (_device == VK_NULL_HANDLE) {
		return;
	}
	vkDestroyDevice(_device, nullptr);
	_device = VK_NULL_HANDLE;
}


/** Command Pool */

void Device::_createCommandPool() {
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(_physicalDevice, _surface);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create command pool");
	}
}

void Device::_destroyCommandPool() {
	if (_commandPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(_device, _commandPool, nullptr);
	}
	_commandPool = VK_NULL_HANDLE;
}

} // namespace Stone::Render::Vulkan
