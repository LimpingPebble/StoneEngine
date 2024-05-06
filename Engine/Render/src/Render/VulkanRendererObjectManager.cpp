// Copyright 2024 Stone-Engine

#include "VulkanRendererObjectManager.hpp"

#include "Utils/FileSystem.hpp"

namespace Stone::Render {

VulkanRendererObjectManager::VulkanRendererObjectManager(const std::shared_ptr<VulkanRenderer> &renderer)
	: _renderer(renderer) {
}

void VulkanRendererObjectManager::updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) {
	RendererObjectManager::updateMeshNode(meshNode);

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

} // namespace Stone::Render
