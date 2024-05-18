// Copyright 2024 Stone-Engine

#include "RendererObjectManager.hpp"

#include "Render/Vulkan/VulkanRenderer.hpp"
#include "Device.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "Scene/RenderElement/Texture.hpp"
#include "Scene/RenderElement/Shader.hpp"
#include "VulkanRenderableNode/MeshNode.hpp"
#include "VulkanRenderableNode/Texture.hpp"
#include "VulkanRenderableNode/Shader.hpp"

namespace Stone::Render::Vulkan {

RendererObjectManager::RendererObjectManager(const std::shared_ptr<VulkanRenderer> &renderer)
	: Scene::RendererObjectManager(), _renderer(renderer) {
}

void RendererObjectManager::updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) {
	Scene::RendererObjectManager::updateMeshNode(meshNode);

	if (meshNode->getRendererObject<Vulkan::MeshNode>()) {
		return;
	}

	auto newMeshNode = std::make_shared<Vulkan::MeshNode>(meshNode, _renderer);
	setRendererObjectTo(meshNode.get(), newMeshNode);
}

void RendererObjectManager::updateTexture(const std::shared_ptr<Scene::Texture> &texture) {
	Scene::RendererObjectManager::updateTexture(texture);

	if (texture->getRendererObject<Vulkan::Texture>()) {
		return;
	}

	auto newTexture = std::make_shared<Vulkan::Texture>(texture, _renderer);
	setRendererObjectTo(texture.get(), newTexture);
}

} // namespace Stone::Render::Vulkan
