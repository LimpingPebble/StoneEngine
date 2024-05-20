// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Image/ImageTypes.hpp"
#include "Scene/Renderable/Texture.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

VkFormat imageChannelToVkFormat(Image::Channel channel);

VkFilter textureFilterToVkFilter(Scene::TextureFilter filter);

VkSamplerAddressMode textureWrapToVkSamplerAddressMode(Scene::TextureWrap wrap);

} // namespace Stone::Render::Vulkan
