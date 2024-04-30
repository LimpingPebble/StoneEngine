// Copyright 2024 Stone-Engine

#include "Render/VulkanRenderer.hpp"

#include "VulkanUtilities.hpp"

#include <iostream>

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

VulkanRenderer::VulkanRenderer(Settings settings)
	: Renderer(), _instance(VK_NULL_HANDLE),
#ifndef NDEBUG
	  _debugMessenger(VK_NULL_HANDLE)
#endif
{
	std::cout << "VulkanRenderer created" << std::endl;
	_createInstance(std::move(settings));
#ifndef NDEBUG
	_setupDebugMessenger();
#endif
}

VulkanRenderer::~VulkanRenderer() {
#ifndef NDEBUG
	_destroyDebugMessenger();
#endif
	_destroyInstance();
	std::cout << "VulkanRenderer destroyed" << std::endl;
}

void VulkanRenderer::_createInstance(Settings settings) {
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
	settings.extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#else
	createInfo.enabledLayerCount = 0;
#endif

	settings.extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	createInfo.enabledExtensionCount = static_cast<uint32_t>(settings.extensions.size());
	createInfo.ppEnabledExtensionNames = settings.extensions.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Vulkan instance");
	}

	enumerateExtensions(std::cout);
}

void VulkanRenderer::_destroyInstance() {
	if (_instance == VK_NULL_HANDLE) {
		return;
	}
	vkDestroyInstance(_instance, nullptr);
	_instance = VK_NULL_HANDLE;
}

void VulkanRenderer::_setupDebugMessenger() {
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
}

void VulkanRenderer::_destroyDebugMessenger() {
	if (_debugMessenger == VK_NULL_HANDLE) {
		return;
	}
	auto func =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(_instance, _debugMessenger, nullptr);
	}
}

} // namespace Stone::Render
