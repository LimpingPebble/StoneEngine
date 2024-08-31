// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderable/IMeshObject.hpp"
#include "Scene/Vertex.hpp"

#include <vector>

/**
 * @brief Represents a skin mesh used for rendering in the scene.
 *
 * It provides functionality for updating the render object and accessing the vertices and indices of the mesh.
 */
namespace Stone::Scene {

class ISkinMeshInterface : public IMeshObject {};

class DynamicSkinMesh : public ISkinMeshInterface {
public:
	DynamicSkinMesh() = default;
	DynamicSkinMesh(const DynamicSkinMesh &other) = default;

	~DynamicSkinMesh() override = default;

	/**
	 * @brief Writes the DynamicSkinMesh object to the output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write the closing brace.
	 * @return The output stream after writing the DynamicSkinMesh object.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Updates the render object using the specified RendererObjectManager.
	 *
	 * @param manager The RendererObjectManager used to update the render object.
	 */
	void updateRenderObject(RendererObjectManager &manager) override;

	/**
	 * @brief Returns a const reference to the vector of WeightVertex objects representing the vertices of the mesh.
	 *
	 * @return A const reference to the vector of WeightVertex objects.
	 */
	[[nodiscard]] const std::vector<WeightVertex> &getVertices() const;

	/**
	 * @brief Returns a reference to the vector of WeightVertex objects representing the vertices of the mesh.
	 *
	 * @note Using this method marks the skin mesh as dirty.
	 *
	 * @return A reference to the vector of WeightVertex objects.
	 */
	std::vector<WeightVertex> &verticesRef();

	/**
	 * @brief Returns a const reference to the vector of uint32_t values representing the indices of the mesh.
	 *
	 * @return A const reference to the vector of uint32_t values.
	 */
	[[nodiscard]] const std::vector<uint32_t> &getIndices() const;

	/**
	 * @brief Returns a reference to the vector of uint32_t values representing the indices of the mesh.
	 *
	 * @note Using this method marks the skin mesh as dirty.
	 *
	 * @return A reference to the vector of uint32_t values.
	 */
	std::vector<uint32_t> &indicesRef();

protected:
	std::vector<WeightVertex> _vertices; /**< The vector of weighted vertices. */
	std::vector<uint32_t> _indices;		 /**< The vector of indices. */
};


class StaticSkinMesh : public ISkinMeshInterface {
public:
	StaticSkinMesh() = default;
	StaticSkinMesh(const StaticSkinMesh &other) = default;

	~StaticSkinMesh() override = default;

	/**
	 * @brief Writes the StaticSkinMesh object to the output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write the closing brace.
	 * @return The output stream after writing the StaticSkinMesh object.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Updates the render object using the specified RendererObjectManager.
	 *
	 * @param manager The RendererObjectManager used to update the render object.
	 */
	void updateRenderObject(RendererObjectManager &manager) override;

	/**
	 * @brief Retrieves the source mesh used to generate the static mesh.
	 *
	 * @return The source mesh used to generate the static mesh.
	 */
	[[nodiscard]] const std::shared_ptr<DynamicSkinMesh> &getSourceMesh() const;

	/**
	 * @brief Sets the source mesh used to generate the static mesh.
	 *
	 * @param sourceMesh The source mesh used to generate the static mesh.
	 */
	void setSourceMesh(const std::shared_ptr<DynamicSkinMesh> &sourceMesh);


protected:
	/**
	 * The dynamic mesh used for rendering.
	 * This pointer will be reset by the renderer once the buffers are initialized.
	 */
	std::shared_ptr<DynamicSkinMesh> _dynamicMesh;
};


} // namespace Stone::Scene
