// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/RenderElement/IRenderable.hpp"
#include "SceneTypes.hpp"

namespace Stone::Scene {

/**
 * @class RendererObjectManager
 * @brief Provide basic overridable methods to manage the renderables objects elements in the scene.
 *
 * This class provides basic methods to update the renderer data of classes implementing the IRenderable interface.
 * It will store an instance of IRendererObject in the IRenderable::_rendererObject proprety.
 * Each IRenderable objects will use the appropriate method in this interface to update itself.
 */
class RendererObjectManager {
public:
	virtual ~RendererObjectManager() = default;

	/**
	 * @brief Updates the renderer data for a given mesh node.
	 * @param meshNode The mesh node to be updated.
	 */
	virtual void updateMeshNode(const std::shared_ptr<MeshNode> &meshNode);

	/**
	 * @brief Updates the renderer data for a given instanced mesh node.
	 * @param instancedMeshNode The instanced mesh node to be updated.
	 */
	virtual void updateInstancedMeshNode(const std::shared_ptr<InstancedMeshNode> &instancedMeshNode);

	/**
	 * @brief Updates the renderer data for a given skin mesh node.
	 * @param skinMeshNode The skin mesh node to be updated.
	 */
	virtual void updateSkinMeshNode(const std::shared_ptr<SkinMeshNode> &skinMeshNode);

	/**
	 * @brief Updates the renderer data for a given material.
	 * @param material The material to be updated.
	 */
	virtual void updateMaterial(const std::shared_ptr<Material> &material);

	/**
	 * @brief Updates the renderer data for a given mesh.
	 * @param mesh The mesh to be updated.
	 */
	virtual void updateMesh(const std::shared_ptr<Mesh> &mesh);

	/**
	 * @brief Updates the renderer data for a given skin mesh.
	 * @param skinmesh The skin mesh to be updated.
	 */
	virtual void updateSkinMesh(const std::shared_ptr<SkinMesh> &skinmesh);

	/**
	 * @brief Updates the renderer data for a given texture.
	 * @param texture The texture to be updated.
	 */
	virtual void updateTexture(const std::shared_ptr<Texture> &texture);

	/**
	 * @brief Updates the renderer data for a given shader.
	 * @param shader The shader to be updated.
	 */
	virtual void updateShader(const std::shared_ptr<Shader> &shader);

protected:
	/**
	 * @brief Sets the IRendererObject value to the given IRenderable.
	 *
	 * This class is friend with IRenderable, so it can access the protected method setRendererObject but its
	 * inheriting classes can't.
	 *
	 * @param element The render element to set the renderer object to.
	 * @param rendererObject The renderer object to set.
	 */
	static void setRendererObjectTo(IRenderable *element, const std::shared_ptr<IRendererObject> &rendererObject);

	/**
	 * @brief Marks the given element as undirty.
	 *
	 * This class is friend with IRenderable, so it can access the protected method markUndirty but its
	 * inheriting classes can't.
	 *
	 * @param element The render element to mark as undirty.
	 */
	static void markElementUndirty(IRenderable *element);
};

} // namespace Stone::Scene
