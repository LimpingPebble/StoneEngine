// Copyright 2024 Stone-Engine

#include "Scene/RendererObjectManager.hpp"

#include "Scene.hpp"

namespace Stone::Scene {

void RendererObjectManager::updateMeshNode(const std::shared_ptr<MeshNode> &meshNode) {
	if (meshNode->getMaterial() && meshNode->getMaterial()->isDirty())
		updateMaterial(meshNode->getMaterial());
	if (meshNode->getMesh() && meshNode->getMesh()->isDirty())
		meshNode->getMesh()->updateRenderObject(*this);
	meshNode->markUndirty();
}

void RendererObjectManager::updateInstancedMeshNode(const std::shared_ptr<InstancedMeshNode> &instancedMeshNode) {
	if (instancedMeshNode->getMaterial() && instancedMeshNode->getMaterial()->isDirty())
		updateMaterial(instancedMeshNode->getMaterial());
	if (instancedMeshNode->getMesh() && instancedMeshNode->getMesh()->isDirty())
		instancedMeshNode->getMesh()->updateRenderObject(*this);
	instancedMeshNode->markUndirty();
}

void RendererObjectManager::updateSkinMeshNode(const std::shared_ptr<SkinMeshNode> &skinMeshNode) {
	if (skinMeshNode->getMaterial() && skinMeshNode->getMaterial()->isDirty())
		updateMaterial(skinMeshNode->getMaterial());
	if (skinMeshNode->getSkinMesh() && skinMeshNode->getSkinMesh()->isDirty())
		skinMeshNode->getSkinMesh()->updateRenderObject(*this);
	skinMeshNode->markUndirty();
}

void RendererObjectManager::updateMaterial(const std::shared_ptr<Material> &material) {
	auto vertexShader = material->getVertexShader();
	if (vertexShader && vertexShader->isDirty()) {
		updateShader(vertexShader);
	}
	auto fragmentShader = material->getFragmentShader();
	if (fragmentShader && fragmentShader->isDirty()) {
		updateShader(fragmentShader);
	}
	material->forEachTextures([this](std::pair<const std::string, std::shared_ptr<Texture>> &it) {
		if (it.second->isDirty())
			updateTexture(it.second);
	});
	material->markUndirty();
}

void RendererObjectManager::updateDynamicMesh(const std::shared_ptr<DynamicMesh> &mesh) {
	mesh->markUndirty();
}

void RendererObjectManager::updateStaticMesh(const std::shared_ptr<StaticMesh> &mesh) {
	mesh->markUndirty();
}

void RendererObjectManager::updateDynamicSkinMesh(const std::shared_ptr<DynamicSkinMesh> &skinmesh) {
	skinmesh->markUndirty();
}

void RendererObjectManager::updateStaticSkinMesh(const std::shared_ptr<StaticSkinMesh> &skinmesh) {
	skinmesh->markUndirty();
}

void RendererObjectManager::updateTexture(const std::shared_ptr<Texture> &texture) {
	texture->markUndirty();
}

void RendererObjectManager::updateShader(const std::shared_ptr<Shader> &shader) {
	shader->markUndirty();
}

void RendererObjectManager::setRendererObjectTo(IRenderable *element,
												const std::shared_ptr<IRendererObject> &rendererObject) {
	element->setRendererObject(rendererObject);
}

void RendererObjectManager::markElementUndirty(IRenderable *element) {
	element->markUndirty();
}

} // namespace Stone::Scene
