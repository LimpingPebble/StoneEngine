// Copyright 2024 Stone-Engine

#include "Render/Vulkan/VulkanRenderer.hpp"

#include "Device.hpp"
#include "FramesRenderer.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

namespace Stone::Render::Vulkan {

VulkanRenderer::VulkanRenderer(VulkanSettings &settings) : Renderer() {
	std::cout << "VulkanRenderer created" << std::endl;

	_device = std::make_shared<Device>(settings);

	SwapChainProperties swapChainProperties = _device->createSwapChainProperties(settings.frame_size);

	_renderPass = std::make_shared<RenderPass>(_device, swapChainProperties.surfaceFormat.format);
	_swapChain = std::make_shared<SwapChain>(_device, _renderPass->getRenderPass(), swapChainProperties);
	_framesRenderer = std::make_shared<FramesRenderer>(_device, _swapChain->getImagecount());
	assert(_framesRenderer->getImageCount() == _swapChain->getImagecount());
}

VulkanRenderer::~VulkanRenderer() {
	if (_device) {
		_device->waitIdle();
	}

	_framesRenderer.reset();
	_swapChain.reset();
	_renderPass.reset();
	_device.reset();

	std::cout << "VulkanRenderer destroyed" << std::endl;
}

void VulkanRenderer::updateFrameSize(std::pair<uint32_t, uint32_t> size) {
	_recreateSwapChain(size);
}

void VulkanRenderer::_recreateSwapChain(std::pair<uint32_t, uint32_t> size) {
	if (_device == nullptr) {
		return;
	}

	_device->waitIdle();

	_swapChain.reset();

	SwapChainProperties swapChainSettings = _device->createSwapChainProperties(size);
	_swapChain = std::make_shared<SwapChain>(_device, _renderPass->getRenderPass(), swapChainSettings);

	if (_framesRenderer == nullptr || _framesRenderer->getImageCount() != _swapChain->getImagecount()) {
		_framesRenderer.reset();
		_framesRenderer = std::make_shared<FramesRenderer>(_device, _swapChain->getImagecount());
	}

	assert(_framesRenderer->getImageCount() == _swapChain->getImagecount());
}


} // namespace Stone::Render::Vulkan
