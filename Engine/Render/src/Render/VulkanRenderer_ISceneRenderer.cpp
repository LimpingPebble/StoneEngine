// Copyright 2024 Stone-Engine

#include "Render/VulkanRenderer.hpp"
#include "Scene/ISceneRenderer.hpp"
#include "VulkanRendererObjectManager.hpp"

namespace Stone::Render {

void VulkanRenderer::updateDataForWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	std::shared_ptr<VulkanRendererObjectManager> manager =
		std::make_shared<VulkanRendererObjectManager>(std::dynamic_pointer_cast<VulkanRenderer>(shared_from_this()));
	world->traverseTopDown([this, manager](const std::shared_ptr<Scene::Node> &node) {
		auto renderElement = std::dynamic_pointer_cast<Scene::IRenderElement>(node);
		if (renderElement && renderElement->isDirty()) {
			renderElement->updateRenderObject(manager);
		}
	});
}

void VulkanRenderer::renderWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	Scene::RenderContext context;
	world->initializeRenderContext(context);
	world->render(context);
}

} // namespace Stone::Render
