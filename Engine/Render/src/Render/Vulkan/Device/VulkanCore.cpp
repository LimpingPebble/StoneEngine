// Copyright 2024 Stone-Engine

#include "VulkanCore.hpp"

#include "../Utilities/VulkanUtilities.hpp"

#include <iostream>
#include <set>

namespace Stone::Render::Vulkan {

VKAPI_ATTR VkBool32 VKAPI_CALL validationLayerMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
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

VulkanCore::VulkanCore(RendererSettings &settings) {
	std::cout << "VulkanCore created" << std::endl;
	_createInstance(settings);
	_setupDebugMessenger();
	_createSurface(settings);
	_pickPhysicalDevice(settings);
	_createLogicalDevice(settings);
	_createCommandPool();
}

VulkanCore::~VulkanCore() {
	waitIdle();

	_destroyCommandPool();
	_destroyLogicalDevice();
	_destroySurface();
	_destroyDebugMessenger();
	_destroyInstance();
	std::cout << "VulkanCore destroyed" << std::endl;
}

void VulkanCore::waitIdle() const {
	vkDeviceWaitIdle(_device);
}


/** Instance */

void VulkanCore::_createInstance(RendererSettings &settings) {
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
#ifdef __APPLE__
	createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
	settings.instanceExt.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

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

	createInfo.enabledExtensionCount = static_cast<uint32_t>(settings.instanceExt.size());
	createInfo.ppEnabledExtensionNames = settings.instanceExt.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Vulkan instance");
	}

	// enumerateExtensions(std::cout);
}

void VulkanCore::_destroyInstance() {
	if (_instance == VK_NULL_HANDLE) {
		return;
	}
	vkDestroyInstance(_instance, nullptr);
	_instance = VK_NULL_HANDLE;
}

/** Debug Messenger */

void VulkanCore::_setupDebugMessenger() {
#ifdef VALIDATION_LAYERS
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = &validationLayerMessageCallback;
	createInfo.pUserData = nullptr;

	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(_instance, &createInfo, nullptr, &_debugMessenger);
	} else {
		throw std::runtime_error("Failed to setup debug messenger");
	}
#endif
}

void VulkanCore::_destroyDebugMessenger() {
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

void VulkanCore::_createSurface(RendererSettings &settings) {
	if (settings.createSurface != nullptr) {
		if (settings.createSurface(_instance, nullptr, &_surface) == VK_SUCCESS) {
			return;
		}
	}
	throw std::runtime_error("Failed to create window surface !");
}

void VulkanCore::_destroySurface() {
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

	// TODO: Implement scoring system structure

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

void VulkanCore::_pickPhysicalDevice(RendererSettings &settings) {
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

void VulkanCore::_createLogicalDevice(RendererSettings &settings) {
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

void VulkanCore::_destroyLogicalDevice() {
	if (_device == VK_NULL_HANDLE) {
		return;
	}
	vkDestroyDevice(_device, nullptr);
	_device = VK_NULL_HANDLE;
}


/** Command Pool */

void VulkanCore::_createCommandPool() {
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(_physicalDevice, _surface);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create command pool");
	}
}

void VulkanCore::_destroyCommandPool() {
	if (_commandPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(_device, _commandPool, nullptr);
	}
	_commandPool = VK_NULL_HANDLE;
}

} // namespace Stone::Render::Vulkan
