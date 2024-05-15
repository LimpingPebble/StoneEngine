// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Image/ImageTypes.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

VkFormat imageChannelToVkFormat(Image::Channel channel);

} // namespace Stone::Render::Vulkan
