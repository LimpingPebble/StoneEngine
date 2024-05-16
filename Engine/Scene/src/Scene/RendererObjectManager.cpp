// Copyright 2024 Stone-Engine

#include "Scene/RendererObjectManager.hpp"

#include "Scene.hpp"

namespace Stone::Scene {

void RendererObjectManager::updateMeshNode(const std::shared_ptr<MeshNode> &meshNode) {
	if (meshNode->getMaterial() && meshNode->getMaterial()->isDirty())
		updateMaterial(meshNode->getMaterial());
	if (meshNode->getMesh() && meshNode->getMesh()->isDirty())
		updateMesh(meshNode->getMesh());
	meshNode->markUndirty();
}

void RendererObjectManager::updateInstancedMeshNode(const std::shared_ptr<InstancedMeshNode> &instancedMeshNode) {
	if (instancedMeshNode->getMaterial() && instancedMeshNode->getMaterial()->isDirty())
		updateMaterial(instancedMeshNode->getMaterial());
	if (instancedMeshNode->getMesh() && instancedMeshNode->getMesh()->isDirty())
		updateMesh(instancedMeshNode->getMesh());
	instancedMeshNode->markUndirty();
}

void RendererObjectManager::updateSkinMeshNode(const std::shared_ptr<SkinMeshNode> &skinMeshNode) {
	if (skinMeshNode->getMaterial() && skinMeshNode->getMaterial()->isDirty())
		updateMaterial(skinMeshNode->getMaterial());
	if (skinMeshNode->getSkinMesh() && skinMeshNode->getSkinMesh()->isDirty())
		updateSkinMesh(skinMeshNode->getSkinMesh());
	skinMeshNode->markUndirty();
}

void RendererObjectManager::updateMaterial(const std::shared_ptr<Material> &material) {
	material->forEachTextures([this](std::pair<const std::string, std::shared_ptr<Texture>> &it) {
		if (it.second->isDirty())
			updateTexture(it.second);
	});
	material->markUndirty();
}

void RendererObjectManager::updateMesh(const std::shared_ptr<Mesh> &mesh) {
	mesh->markUndirty();
}

void RendererObjectManager::updateSkinMesh(const std::shared_ptr<SkinMesh> &skinmesh) {
	skinmesh->markUndirty();
}

void RendererObjectManager::updateTexture(const std::shared_ptr<Texture> &texture) {
	texture->markUndirty();
}

void RendererObjectManager::setRendererObjectTo(IRenderElement *element,
												const std::shared_ptr<IRendererObject> &rendererObject) {
	element->setRendererObject(rendererObject);
}

void RendererObjectManager::markElementUndirty(IRenderElement *element) {
	element->markUndirty();
}

} // namespace Stone::Scene
