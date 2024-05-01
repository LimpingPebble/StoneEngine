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
		std::vector<const char *> extensions = {};
		std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
		std::function<VkResult(VkInstance, const VkAllocationCallbacks *, VkSurfaceKHR *)> createSurface = nullptr;
	};

	VulkanRenderer() = delete;
	explicit VulkanRenderer(Settings &settings);
	VulkanRenderer(const VulkanRenderer &) = delete;

	~VulkanRenderer() override;

	void generateDataForMesh(std::shared_ptr<Scene::Mesh> mesh) override;
	void generateDataForSkinMesh(std::shared_ptr<Scene::SkinMesh> skinmesh) override;

	void generateDataForMaterial(std::shared_ptr<Scene::Material> material) override;
	void generateDataForTexture(std::shared_ptr<Scene::Texture> texture) override;

	/** Element Rendering */
	void renderMeshNode(std::shared_ptr<Scene::MeshNode> node, Scene::RenderContext &context) override;
	void renderInstancedMeshNode(std::shared_ptr<Scene::InstancedMeshNode> instancedmesh,
								 Scene::RenderContext &context) override;
	void renderSkinMeshNode(std::shared_ptr<Scene::SkinMeshNode> skinmesh, Scene::RenderContext &context) override;

private:
	void _createInstance(Settings &settings);
	void _destroyInstance();

	void _setupDebugMessenger();
	void _destroyDebugMessenger();

	void _createSurface(Settings &settings);
	void _destroySurface();

	void _pickPhysicalDevice();

	void _createLogicalDevice(Settings &settings);
	void _destroyLogicalDevice();

	VkInstance _instance = VK_NULL_HANDLE;
#ifndef NDEBUG
	VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
#endif
	VkSurfaceKHR _surface = VK_NULL_HANDLE;
	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	VkDevice _device = VK_NULL_HANDLE;
	VkQueue _graphicsQueue = VK_NULL_HANDLE;
	VkQueue _presentQueue = VK_NULL_HANDLE;
};

} // namespace Stone::Render
