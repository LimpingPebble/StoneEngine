// Copyright 2024 Stone-Engine

#pragma once

#include "../RenderContext.hpp"
#include "Scene/RenderElement/IRenderElement.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Scene {
class Texture;
} // namespace Stone::Scene

namespace Stone::Render::Vulkan {

class Device;
class RenderPass;
class SwapChain;

class Texture : public Scene::IRendererObject {
public:
	Texture(const std::shared_ptr<Scene::Texture> &texture, const std::shared_ptr<Device> &device,
			const std::shared_ptr<RenderPass> &renderPass, const std::shared_ptr<SwapChain> &swapChain);

	~Texture() override;

	void render(Scene::RenderContext &context) override;

private:
	void _createTextureImage();
	void _destroyTextureImage();

	void _createTextureImageView();
	void _destroyTextureImageView();

	void _createTextureSampler();
	void _destroyTextureSampler();

	std::shared_ptr<Device> _device;

	std::weak_ptr<Scene::Texture> _sceneTexture;

	VkImage _textureImage = VK_NULL_HANDLE;
	VkDeviceMemory _textureImageMemory = VK_NULL_HANDLE;

	VkImageView _textureImageView = VK_NULL_HANDLE;

	VkSampler _textureSampler = VK_NULL_HANDLE;
};

} // namespace Stone::Render::Vulkan
