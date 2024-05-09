// Copyright 2024 StoneEngine

#pragma once

#include "Scene/RenderContext.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

struct RenderContext : public Scene::RenderContext {
	VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
	VkExtent2D extent = {};
};

} // namespace Stone::Render::Vulkan
