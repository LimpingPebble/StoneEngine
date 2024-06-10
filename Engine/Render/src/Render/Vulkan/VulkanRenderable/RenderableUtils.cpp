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

VkFilter textureFilterToVkFilter(Scene::TextureFilter filter) {
	switch (filter) {
	case Scene::TextureFilter::Nearest: return VK_FILTER_NEAREST;
	case Scene::TextureFilter::Linear: return VK_FILTER_LINEAR;
	case Scene::TextureFilter::Cubic: return VK_FILTER_CUBIC_IMG;
	default: throw std::runtime_error("Unsupported texture filter");
	}
}

VkSamplerAddressMode textureWrapToVkSamplerAddressMode(Scene::TextureWrap wrap) {
	switch (wrap) {
	case Scene::TextureWrap::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
	case Scene::TextureWrap::MirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
	case Scene::TextureWrap::ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	case Scene::TextureWrap::ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	default: throw std::runtime_error("Unsupported texture wrap mode");
	}
}

} // namespace Stone::Render::Vulkan
