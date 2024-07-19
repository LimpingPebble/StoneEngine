// Copyright 2024 Stone-Engine

#pragma once

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

struct SwapChainProperties {
	VkSurfaceFormatKHR surfaceFormat = {};
	VkPresentModeKHR presentMode = {};
	VkSurfaceCapabilitiesKHR capabilities = {};
	VkExtent2D extent = {};
	uint32_t minImageCount = 0;
	uint32_t imageCount = 0;
};

} // namespace Stone::Render::Vulkan
