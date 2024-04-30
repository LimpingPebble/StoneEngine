// Copyright 2024 Stone-Engine

#include "Render/VulkanRenderer.hpp"

namespace Stone::Render {

void VulkanRenderer::generateDataForMesh(std::shared_ptr<Scene::Mesh> mesh) {
	(void)mesh;
}

void VulkanRenderer::generateDataForSkinMesh(std::shared_ptr<Scene::SkinMesh> skinmesh) {
	(void)skinmesh;
}

void VulkanRenderer::generateDataForMaterial(std::shared_ptr<Scene::Material> material) {
	(void)material;
}

void VulkanRenderer::generateDataForTexture(std::shared_ptr<Scene::Texture> texture) {
	(void)texture;
}

void VulkanRenderer::renderMeshNode(std::shared_ptr<Scene::MeshNode> node, Scene::RenderContext &context) {
	(void)node;
	(void)context;
}

void VulkanRenderer::renderInstancedMeshNode(std::shared_ptr<Scene::InstancedMeshNode> instancedmesh,
											 Scene::RenderContext &context) {
	(void)instancedmesh;
	(void)context;
}

void VulkanRenderer::renderSkinMeshNode(std::shared_ptr<Scene::SkinMeshNode> skinmesh, Scene::RenderContext &context) {
	(void)skinmesh;
	(void)context;
}

} // namespace Stone::Render