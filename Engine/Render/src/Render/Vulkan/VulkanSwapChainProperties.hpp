// Copyright 2024 Stone-Engine

#pragma once

#include <vulkan/vulkan.h>

struct VulkanSwapChainProperties {
	VkSurfaceFormatKHR surfaceFormat = {};
	VkPresentModeKHR presentMode = {};
	VkSurfaceCapabilitiesKHR capabilities = {};
	VkExtent2D extent = {};
	uint32_t minImageCount = 0;
	uint32_t imageCount = 0;
};
