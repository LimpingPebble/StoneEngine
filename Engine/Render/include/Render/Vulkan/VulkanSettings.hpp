// Copyright 2024 Stone-Engine

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#ifndef NDEBUG
#define VALIDATION_LAYERS
#endif

namespace Stone::Render::Vulkan {

struct VulkanSettings {
	std::string app_name = "Stone";
	uint32_t app_version = VK_MAKE_VERSION(1, 0, 0);
	std::vector<const char *> instanceExt = {};
	std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
	std::function<VkResult(VkInstance, const VkAllocationCallbacks *, VkSurfaceKHR *)> createSurface = nullptr;
	std::vector<const char *> deviceExt = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	std::pair<uint32_t, uint32_t> frame_size = {};
};

} // namespace Stone::Render::Vulkan
