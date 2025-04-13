// Copyright 2024 Stone-Engine

#include "Scene/Renderer/RendererObjectFactory.hpp"

#include "Scene.hpp"

namespace Stone::Scene {

using CastFunction = std::function<void(RendererObjectFactory &, const std::shared_ptr<Core::Object> &)>;

#define CASTED_FUNCTION_MAP_ENTRY(ClassName)                                                                           \
	{ClassName::StaticHashCode(),                                                                                      \
	 [](RendererObjectFactory &factory, const std::shared_ptr<Core::Object> &renderable) {                             \
		 factory.update##ClassName(std::static_pointer_cast<ClassName>(renderable));                                   \
	 }}

const std::unordered_map<std::intptr_t, CastFunction> updateCastedFunctions = {
	CASTED_FUNCTION_MAP_ENTRY(MeshNode),		CASTED_FUNCTION_MAP_ENTRY(InstancedMeshNode),
	CASTED_FUNCTION_MAP_ENTRY(SkinMeshNode),	CASTED_FUNCTION_MAP_ENTRY(Material),
	CASTED_FUNCTION_MAP_ENTRY(DynamicMesh),		CASTED_FUNCTION_MAP_ENTRY(StaticMesh),
	CASTED_FUNCTION_MAP_ENTRY(DynamicSkinMesh), CASTED_FUNCTION_MAP_ENTRY(StaticSkinMesh),
	CASTED_FUNCTION_MAP_ENTRY(WireframeShape),	CASTED_FUNCTION_MAP_ENTRY(Texture),
	CASTED_FUNCTION_MAP_ENTRY(FragmentShader),
};

void RendererObjectFactory::updateRenderable(const std::shared_ptr<Core::Object> &renderable) {
	auto it = updateCastedFunctions.find(renderable->getClassHashCode());
	if (it != updateCastedFunctions.end()) {
		it->second(*this, renderable);
	}
}

void RendererObjectFactory::updateMeshNode(const std::shared_ptr<MeshNode> &meshNode) {
	if (meshNode->getMaterial() && meshNode->getMaterial()->isDirty())
		updateMaterial(meshNode->getMaterial());
	if (meshNode->getMesh() && meshNode->getMesh()->isDirty())
		updateRenderable(meshNode->getMesh());
}

void RendererObjectFactory::updateInstancedMeshNode(const std::shared_ptr<InstancedMeshNode> &instancedMeshNode) {
	if (instancedMeshNode->getMaterial() && instancedMeshNode->getMaterial()->isDirty())
		updateMaterial(instancedMeshNode->getMaterial());
	if (instancedMeshNode->getMesh() && instancedMeshNode->getMesh()->isDirty())
		updateRenderable(instancedMeshNode->getMesh());
}

void RendererObjectFactory::updateSkinMeshNode(const std::shared_ptr<SkinMeshNode> &skinMeshNode) {
	if (skinMeshNode->getMaterial() && skinMeshNode->getMaterial()->isDirty())
		updateMaterial(skinMeshNode->getMaterial());
	if (skinMeshNode->getSkinMesh() && skinMeshNode->getSkinMesh()->isDirty())
		updateRenderable(skinMeshNode->getSkinMesh());
}

void RendererObjectFactory::updateMaterial(const std::shared_ptr<Material> &material) {
	auto fragmentShader = material->getFragmentShader();
	if (fragmentShader && fragmentShader->isDirty()) {
		updateFragmentShader(fragmentShader);
	}
	material->forEachTextures([this](const Material::Location &loc, const std::shared_ptr<Texture> &texture) {
		(void)loc;
		if (texture->isDirty())
			updateTexture(texture);
	});
}

void RendererObjectFactory::updateDynamicMesh(const std::shared_ptr<DynamicMesh> &mesh) {
	if (mesh->getDefaultMaterial() && mesh->getDefaultMaterial()->isDirty())
		updateMaterial(mesh->getDefaultMaterial());
}

void RendererObjectFactory::updateStaticMesh(const std::shared_ptr<StaticMesh> &mesh) {
	if (mesh->getDefaultMaterial() && mesh->getDefaultMaterial()->isDirty())
		updateMaterial(mesh->getDefaultMaterial());
}

void RendererObjectFactory::updateDynamicSkinMesh(const std::shared_ptr<DynamicSkinMesh> &skinmesh) {
	if (skinmesh->getDefaultMaterial() && skinmesh->getDefaultMaterial()->isDirty())
		updateMaterial(skinmesh->getDefaultMaterial());
}

void RendererObjectFactory::updateStaticSkinMesh(const std::shared_ptr<StaticSkinMesh> &skinmesh) {
	if (skinmesh->getDefaultMaterial() && skinmesh->getDefaultMaterial()->isDirty())
		updateMaterial(skinmesh->getDefaultMaterial());
}

void RendererObjectFactory::updateWireframeShape(const std::shared_ptr<WireframeShape> &shape) {
	(void)shape;
}

void RendererObjectFactory::updateTexture(const std::shared_ptr<Texture> &texture) {
	(void)texture;
}

void RendererObjectFactory::updateFragmentShader(const std::shared_ptr<FragmentShader> &shader) {
	(void)shader;
}

void RendererObjectFactory::updateRendererObject(IRenderable &element,
												 const std::shared_ptr<IRendererObject> &rendererObject) {
	element.setRendererObject(rendererObject);
	element.markUndirty();
}

} // namespace Stone::Scene
