// Copyright 2024 Stone-Engine

#include "Render/VulkanRenderer.hpp"

namespace Stone::Render {

VulkanRenderer::VulkanRenderer(Settings settings) : Renderer() {
	std::cout << "VulkanRenderer created" << std::endl;
	_createInstance(std::move(settings));
}

VulkanRenderer::~VulkanRenderer() {
	std::cout << "VulkanRenderer destroyed" << std::endl;
	vkDestroyInstance(_instance, nullptr);
}

static void enumerateExtensions() {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "Available extensions:" << std::endl;
	for (const auto &extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}
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
	createInfo.enabledLayerCount = 0;

	settings.extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	createInfo.enabledExtensionCount = static_cast<uint32_t>(settings.extensions.size());
	createInfo.ppEnabledExtensionNames = settings.extensions.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Vulkan instance");
	}

	enumerateExtensions();
}


} // namespace Stone::Render
