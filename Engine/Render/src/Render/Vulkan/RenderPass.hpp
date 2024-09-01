// Copyright 2024 Stone-Engine

#pragma once

#include <memory>
#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

class Device;

class RenderPass {
public:
	RenderPass() = delete;
	RenderPass(const std::shared_ptr<Device> &device, VkFormat format);
	RenderPass(const RenderPass &) = delete;

	virtual ~RenderPass();

	[[nodiscard]] const VkRenderPass &getRenderPass() const {
		return _renderPass;
	}

	[[nodiscard]] const VkFormat &getFormat() const {
		return _format;
	}

private:
	void _createRenderPass();
	void _destroyRenderPass();

	std::shared_ptr<Device> _device;

	VkRenderPass _renderPass = VK_NULL_HANDLE;
	VkFormat _format = VK_FORMAT_UNDEFINED;
};

} // namespace Stone::Render::Vulkan
