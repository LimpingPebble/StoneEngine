// Copyright 2024 Stone-Engine

#pragma once

#include "Render/Renderer.hpp"
#include "Scene.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render {

class VulkanRenderer : public Renderer {
public:
	struct Settings {
		std::string app_name = "Stone";
		uint32_t app_version = VK_MAKE_VERSION(1, 0, 0);
		std::vector<const char *> instanceExt = {};
		std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
		std::function<VkResult(VkInstance, const VkAllocationCallbacks *, VkSurfaceKHR *)> createSurface = nullptr;
		std::vector<const char *> deviceExt = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
		std::pair<uint32_t, uint32_t> frame_size = {};
	};

	VulkanRenderer() = delete;
	explicit VulkanRenderer(Settings &settings);
	VulkanRenderer(const VulkanRenderer &) = delete;

	~VulkanRenderer() override;

	/** Renderer */

	void updateDataForWorld(const std::shared_ptr<Scene::WorldNode> &world) override;
	void renderWorld(const std::shared_ptr<Scene::WorldNode> &world) override;

	void updateFrameSize(std::pair<uint32_t, uint32_t> size) override;


private:
	void _createInstance(Settings &settings);
	void _destroyInstance();

	void _setupDebugMessenger();
	void _destroyDebugMessenger();

	void _createSurface(Settings &settings);
	void _destroySurface();

	void _pickPhysicalDevice(Settings &settings);

	void _createLogicalDevice(Settings &settings);
	void _destroyLogicalDevice();

	void _createSwapChain(const std::pair<uint32_t, uint32_t> &size);
	void _destroySwapChain();

	void _createImageViews();
	void _destroyImageViews();

	void _createRenderPass();
	void _destroyRenderPass();

	void _createGraphicPipeline();
	void _destroyGraphicPipeline();

	VkShaderModule _createShaderModule(const std::vector<char> &code);

	void _createFramebuffers();
	void _destroyFramebuffers();

	void _createCommandPool();
	void _destroyCommandPool();

	VkInstance _instance = VK_NULL_HANDLE;
#ifndef NDEBUG
	VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
#endif
	VkSurfaceKHR _surface = VK_NULL_HANDLE;
	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	VkDevice _device = VK_NULL_HANDLE;
	VkQueue _graphicsQueue = VK_NULL_HANDLE;
	VkQueue _presentQueue = VK_NULL_HANDLE;
	VkSwapchainKHR _swapChain = VK_NULL_HANDLE;
	std::vector<VkImage> _swapChainImages = {};
	VkFormat _swapChainImageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D _swapChainExtent = {0, 0};
	std::vector<VkImageView> _swapChainImageViews = {};
	VkRenderPass _renderPass = VK_NULL_HANDLE;
	VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
	VkPipeline _graphicsPipeline = VK_NULL_HANDLE;
	std::vector<VkFramebuffer> _swapChainFramebuffers = {};
	VkCommandPool _commandPool = VK_NULL_HANDLE;
};

} // namespace Stone::Render
