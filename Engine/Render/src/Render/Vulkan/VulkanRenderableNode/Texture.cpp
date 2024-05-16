// Copyright 2024 Stone-Engine

#include "Texture.hpp"

#include "../Device.hpp"
#include "../RenderContext.hpp"
#include "../RenderPass.hpp"
#include "../SwapChain.hpp"
#include "Core/Image/ImageData.hpp"
#include "Core/Image/ImageSource.hpp"
#include "RenderableUtils.hpp"
#include "Scene/RenderElement/Texture.hpp"

#include <cstring>

namespace Stone::Render::Vulkan {

Texture::Texture(const std::shared_ptr<Scene::Texture> &texture, const std::shared_ptr<Device> &device,
				 const std::shared_ptr<RenderPass> &renderPass, const std::shared_ptr<SwapChain> &swapChain)
	: _device(device), _sceneTexture(texture) {
	(void)renderPass;
	(void)swapChain;

	_createTextureImage();
	_createTextureImageView();
	std::cout << "texture created" << std::endl;
}

Texture::~Texture() {
	_destroyTextureImageView();
	_destroyTextureImage();
	std::cout << "texture destroyed" << std::endl;
}

void Texture::render(Scene::RenderContext &context) {
	(void)context;
}

void Texture::_createTextureImage() {
	auto texture = _sceneTexture.lock();
	const std::shared_ptr<Image::ImageData> &image = texture->getImage()->getLoadedImage(true);

	VkDeviceSize imageSize = image->getSize().x * image->getSize().y * static_cast<int>(image->getChannels());
	auto [stagingBuffer, stagingBufferMemory] =
		_device->createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
							  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void *data;
	vkMapMemory(_device->getDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
	std::memcpy(data, image->getData(), static_cast<size_t>(imageSize));
	vkUnmapMemory(_device->getDevice(), stagingBufferMemory);

	texture->getImage()->unloadData();

	std::tie(_textureImage, _textureImageMemory) = _device->createImage(
		image->getSize().x, image->getSize().y, 1, VK_SAMPLE_COUNT_1_BIT, imageChannelToVkFormat(image->getChannels()),
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	_device->transitionImageLayout(_textureImage, imageChannelToVkFormat(image->getChannels()),
								   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	_device->copyBufferToImage(stagingBuffer, _textureImage, image->getSize().x, image->getSize().y);

	_device->transitionImageLayout(_textureImage, imageChannelToVkFormat(image->getChannels()),
								   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(_device->getDevice(), stagingBuffer, nullptr);
	vkFreeMemory(_device->getDevice(), stagingBufferMemory, nullptr);
}

void Texture::_destroyTextureImage() {
	vkDestroyImage(_device->getDevice(), _textureImage, nullptr);
	vkFreeMemory(_device->getDevice(), _textureImageMemory, nullptr);
}

void Texture::_createTextureImageView() {
	auto texture = _sceneTexture.lock();
	const std::shared_ptr<Image::ImageData> &image = texture->getImage()->getLoadedImage(true);

	_textureImageView = _device->createImageView(_textureImage, imageChannelToVkFormat(image->getChannels()));
}

void Texture::_destroyTextureImageView() {
	vkDestroyImageView(_device->getDevice(), _textureImageView, nullptr);
}

} // namespace Stone::Render::Vulkan
