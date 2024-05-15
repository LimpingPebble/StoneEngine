// Copyright 2024 Stone-Engine

#include "RenderableUtils.hpp"

namespace Stone::Render::Vulkan {

VkFormat imageChannelToVkFormat(Image::Channel channel) {
	switch (channel) {
	case Image::Channel::GREY: return VK_FORMAT_R8_UNORM;
	case Image::Channel::DUAL: return VK_FORMAT_R8G8_UNORM;
	case Image::Channel::RGB: return VK_FORMAT_R8G8B8_UNORM;
	case Image::Channel::RGBA: return VK_FORMAT_R8G8B8A8_UNORM;
	default: throw std::runtime_error("Unsupported channel format");
	}
}

} // namespace Stone::Render::Vulkan
