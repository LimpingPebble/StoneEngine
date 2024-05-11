// Copyright 2024 Stone-Engine

#include "RendererObjectManager.hpp"

#include "Device.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "VulkanRenderableNode/MeshNode.hpp"

namespace Stone::Render::Vulkan {

RendererObjectManager::RendererObjectManager(const std::shared_ptr<Device> &device,
											 const std::shared_ptr<RenderPass> &renderPass,
											 const std::shared_ptr<SwapChain> &swapChain)
	: Scene::RendererObjectManager(), _device(device), _renderPass(renderPass), _swapChain(swapChain) {
}

void RendererObjectManager::updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) {
	Scene::RendererObjectManager::updateMeshNode(meshNode);

	if (meshNode->getRendererObject<Vulkan::MeshNode>()) {
		return;
	}

	auto newMeshNode = std::make_shared<Vulkan::MeshNode>(meshNode, _device, _renderPass, _swapChain);
	setRendererObjectTo(meshNode.get(), newMeshNode);
}

} // namespace Stone::Render::Vulkan
