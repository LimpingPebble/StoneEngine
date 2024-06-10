// Copyright 2024 Stone-Engine

#pragma once

#include "Render/VulkanRenderer.hpp"
#include "Scene/RendererObjectManager.hpp"

namespace Stone::Render {

class VulkanRendererObjectManager : public Scene::RendererObjectManager {

public:
	explicit VulkanRendererObjectManager(const std::shared_ptr<VulkanRenderer> &renderer);

	~VulkanRendererObjectManager() override = default;

	void updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) override;

private:
	std::shared_ptr<VulkanRenderer> _renderer;
};

class VulkanMeshNode : public Scene::IRendererObject {
public:
	VulkanMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode, const std::shared_ptr<VulkanRenderer> &renderer);

	~VulkanMeshNode() override = default;

	void render(Scene::RenderContext &context) override;
};

} // namespace Stone::Render
