// Copyright 2024 Stone-Engine

#include "RendererObjectManager.hpp"

#include "Scene/Node/MeshNode.hpp"
#include "Utils/FileSystem.hpp"

namespace Stone::Render::Vulkan {

RendererObjectManager::RendererObjectManager(const std::shared_ptr<VulkanRenderer> &renderer) : _renderer(renderer) {
}

void RendererObjectManager::updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) {
	Scene::RendererObjectManager::updateMeshNode(meshNode);

	if (meshNode->getRendererObject<VulkanMeshNode>()) {
		return;
	}

	setRendererObjectTo(meshNode.get(), std::make_shared<VulkanMeshNode>(meshNode, _renderer));
}

VulkanMeshNode::VulkanMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode,
							   const std::shared_ptr<VulkanRenderer> &renderer) {
	(void)meshNode;
	(void)renderer;
}

void VulkanMeshNode::render(Scene::RenderContext &context) {
	(void)context;
}

} // namespace Stone::Render::Vulkan
