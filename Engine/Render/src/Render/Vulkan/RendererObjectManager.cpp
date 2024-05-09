// Copyright 2024 Stone-Engine

#include "RendererObjectManager.hpp"

#include "Device.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "VulkanRenderableNode/MeshNode.hpp"

namespace Stone::Render::Vulkan {

RendererObjectManager::RendererObjectManager(const std::shared_ptr<Device> &device,
											 const std::shared_ptr<RenderPass> &renderPass, VkExtent2D extent)
	: Scene::RendererObjectManager(), _device(device), _renderPass(renderPass), _extent(extent) {
}

void RendererObjectManager::updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) {
	Scene::RendererObjectManager::updateMeshNode(meshNode);

	if (meshNode->getRendererObject<Vulkan::MeshNode>()) {
		return;
	}

	auto newMeshNode = std::make_shared<Vulkan::MeshNode>(meshNode, _device, _renderPass, _extent);
	setRendererObjectTo(meshNode.get(), newMeshNode);
}

} // namespace Stone::Render::Vulkan
