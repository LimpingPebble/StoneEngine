// Copyright 2024 Stone-Engine

#include "Scene/RendererObjectManager.hpp"

#include "Scene.hpp"

namespace Stone::Scene {

void RendererObjectManager::updateMeshNode(const std::shared_ptr<MeshNode> &meshNode) {
	if (meshNode->getMesh() && meshNode->getMesh()->isDirty())
		updateMesh(meshNode->getMesh());
	if (meshNode->getMaterial() && meshNode->getMaterial()->isDirty())
		updateMaterial(meshNode->getMaterial());
	meshNode->markUndirty();
}

void RendererObjectManager::updateInstancedMeshNode(const std::shared_ptr<InstancedMeshNode> &instancedMeshNode) {
	if (instancedMeshNode->getMesh() && instancedMeshNode->getMesh()->isDirty())
		updateMesh(instancedMeshNode->getMesh());
	if (instancedMeshNode->getMaterial() && instancedMeshNode->getMaterial()->isDirty())
		updateMaterial(instancedMeshNode->getMaterial());
	instancedMeshNode->markUndirty();
}

void RendererObjectManager::updateSkinMeshNode(const std::shared_ptr<SkinMeshNode> &skinMeshNode) {
	if (skinMeshNode->getSkinMesh() && skinMeshNode->getSkinMesh()->isDirty())
		updateSkinMesh(skinMeshNode->getSkinMesh());
	if (skinMeshNode->getMaterial() && skinMeshNode->getMaterial()->isDirty())
		updateMaterial(skinMeshNode->getMaterial());
	skinMeshNode->markUndirty();
}

void RendererObjectManager::updateMaterial(const std::shared_ptr<Material> &material) {
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

std::shared_ptr<IRendererObject> RendererObjectManager::getRendererObjectFrom(IRenderElement *element) {
	return element->getRendererObject();
}

void RendererObjectManager::setRendererObjectTo(IRenderElement *element,
												const std::shared_ptr<IRendererObject> &rendererObject) {
	element->setRendererObject(rendererObject);
}

void RendererObjectManager::markElementUndirty(IRenderElement *element) {
	element->markUndirty();
}

} // namespace Stone::Scene
