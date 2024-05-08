// Copyright 2024 Stone-Engine

#pragma once

#include <memory>
#include <vulkan/vulkan.h>

namespace Stone::Render {

class VulkanDevice;

class VulkanRenderPass {
public:
	VulkanRenderPass() = delete;
	VulkanRenderPass(const std::shared_ptr<VulkanDevice> &device, VkFormat format);
	VulkanRenderPass(const VulkanRenderPass &) = delete;

	virtual ~VulkanRenderPass();

	[[nodiscard]] const VkRenderPass &getRenderPass() const {
		return _renderPass;
	}

	[[nodiscard]] const VkFormat &getFormat() const {
		return _format;
	}

private:
	void _createRenderPass();
	void _destroyRenderPass();

	std::shared_ptr<VulkanDevice> _device;

	VkRenderPass _renderPass = VK_NULL_HANDLE;
	VkFormat _format = VK_FORMAT_UNDEFINED;
};

} // namespace Stone::Render
