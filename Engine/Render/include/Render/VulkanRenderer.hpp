// Copyright 2024 Stone-Engine

#pragma once

#include "Render/Renderer.hpp"
#include "Scene.hpp"
#include "vulkan/vulkan.h"

namespace Stone::Render {

class VulkanRenderer : public Renderer {
public:
	struct Settings {
		std::string app_name = "Stone";
		uint32_t app_version = VK_MAKE_VERSION(1, 0, 0);
		std::vector<const char *> extensions;
		std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
	};

	VulkanRenderer() = delete;
	explicit VulkanRenderer(Settings settings);
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
	void _createInstance(Settings settings);
	void _destroyInstance();

	void _setupDebugMessenger();
	void _destroyDebugMessenger();

	VkInstance _instance;
#ifndef NDEBUG
	VkDebugUtilsMessengerEXT _debugMessenger;
#endif
};

} // namespace Stone::Render
