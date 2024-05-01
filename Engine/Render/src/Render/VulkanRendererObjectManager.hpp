// Copyright 2024 Stone-Engine

#pragma once

#include "Render/VulkanRenderer.hpp"
#include "Scene/RendererObjectManager.hpp"

namespace Stone::Render {

class VulkanRendererObjectManager : public Scene::RendererObjectManager {

public:
	explicit VulkanRendererObjectManager(const std::shared_ptr<VulkanRenderer> &renderer) : _renderer(renderer) {
	}

	void updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) override {
		RendererObjectManager::updateMeshNode(meshNode);
		std::cout << "MeshNode updated " << meshNode->getName() << std::endl;
	}

	void updateMesh(const std::shared_ptr<Scene::Mesh> &mesh) override {
		RendererObjectManager::updateMesh(mesh);
		std::cout << "Mesh updated " << mesh->getId() << std::endl;
	}

	void updateMaterial(const std::shared_ptr<Scene::Material> &material) override {
		RendererObjectManager::updateMaterial(material);
		std::cout << "Material updated " << material->getId() << std::endl;
	}

private:
	std::shared_ptr<VulkanRenderer> _renderer;
};

} // namespace Stone::Render
