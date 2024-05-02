// Copyright 2024 Stone-Engine

#include "Render/VulkanRenderer.hpp"

#include "Utils/FileSystem.hpp"
#include "VulkanUtilities.hpp"

#include <iostream>
#include <set>

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

namespace Stone::Render {

VulkanRenderer::VulkanRenderer(Settings &settings) : Renderer() {
	std::cout << "VulkanRenderer created" << std::endl;
	_createInstance(settings);
	_setupDebugMessenger();
	_createSurface(settings);
	_pickPhysicalDevice(settings);
	_createLogicalDevice(settings);
	_createSwapChain(settings.frame_size);
	_createImageViews();
	_createGraphicPipeline();
}

VulkanRenderer::~VulkanRenderer() {
	_destroyGraphicPipeline();
	_destroyImageViews();
	_destroySwapChain();
	_destroyLogicalDevice();
	_destroySurface();
	_destroyDebugMessenger();
	_destroyInstance();
	std::cout << "VulkanRenderer destroyed" << std::endl;
}

void VulkanRenderer::updateFrameSize(std::pair<uint32_t, uint32_t> size) {
	vkDeviceWaitIdle(_device);

	_destroyImageViews();
	_destroySwapChain();
	_createSwapChain(size);
	_createImageViews();
}

/** Instance */

void VulkanRenderer::_createInstance(Settings &settings) {
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

#ifndef NDEBUG
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

void VulkanRenderer::_destroyInstance() {
	if (_instance == VK_NULL_HANDLE) {
		return;
	}
	vkDestroyInstance(_instance, nullptr);
	_instance = VK_NULL_HANDLE;
}

/** Debug Messenger */

void VulkanRenderer::_setupDebugMessenger() {
#ifndef NDEBUG
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

void VulkanRenderer::_destroyDebugMessenger() {
#ifndef NDEBUG
	if (_debugMessenger == VK_NULL_HANDLE) {
		return;
	}
	auto func =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(_instance, _debugMessenger, nullptr);
	}
#endif
}

/** Surface */

void VulkanRenderer::_createSurface(Settings &settings) {
	if (settings.createSurface != nullptr) {
		if (settings.createSurface(_instance, nullptr, &_surface) == VK_SUCCESS) {
			return;
		}
	}
	throw std::runtime_error("Failed to create window surface !");
}

void VulkanRenderer::_destroySurface() {
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
	if (!features.geometryShader) {
		return -1;
	}

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

void VulkanRenderer::_pickPhysicalDevice(Settings &settings) {
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

void VulkanRenderer::_createLogicalDevice(Settings &settings) {
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

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = settings.deviceExt.size();
	createInfo.ppEnabledExtensionNames = settings.deviceExt.data();

#ifndef NDEBUG
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

void VulkanRenderer::_destroyLogicalDevice() {
	if (_device == VK_NULL_HANDLE) {
		return;
	}
	vkDestroyDevice(_device, nullptr);
	_device = VK_NULL_HANDLE;
}

/** Swap chain */

void VulkanRenderer::_createSwapChain(const std::pair<uint32_t, uint32_t> &size) {
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(_physicalDevice, _surface);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

	auto [width, height] = size;
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = _surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(_physicalDevice, _surface);
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

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create swap chain");
	}

	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());

	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = extent;
}

void VulkanRenderer::_destroySwapChain() {
	if (_swapChain == VK_NULL_HANDLE) {
		return;
	}
	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
	_swapChain = VK_NULL_HANDLE;
}

/** Image Views */

void VulkanRenderer::_createImageViews() {
	_swapChainImageViews.resize(_swapChainImages.size());

	for (size_t i = 0; i < _swapChainImages.size(); ++i) {
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = _swapChainImages[i];

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = _swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(_device, &createInfo, nullptr, &_swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image views");
		}
	}
}

void VulkanRenderer::_destroyImageViews() {
	for (auto imageView : _swapChainImageViews) {
		vkDestroyImageView(_device, imageView, nullptr);
	}
	_swapChainImageViews.clear();
}

void VulkanRenderer::_createGraphicPipeline() {
	auto vertShaderCode = Utils::readFile("shaders/vert.spv");
	auto fragShaderCode = Utils::readFile("shaders/frag.spv");

	auto vertShaderModule = _createShaderModule(vertShaderCode);
	auto fragShaderModule = _createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	vkDestroyShaderModule(_device, vertShaderModule, nullptr);
	vkDestroyShaderModule(_device, fragShaderModule, nullptr);
}

void VulkanRenderer::_destroyGraphicPipeline() {
}

VkShaderModule VulkanRenderer::_createShaderModule(const std::vector<char> &code) {
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

} // namespace Stone::Render
