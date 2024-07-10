// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/Renderable/IRenderable.hpp"
#include "Scene/Vertex.hpp"

#include <vector>

/**
 * @brief Represents a skin mesh used for rendering in the scene.
 *
 * It provides functionality for updating the render object and accessing the vertices and indices of the mesh.
 */
namespace Stone::Scene {

class SkinMesh : public Core::Object, public IRenderable {
public:
	SkinMesh() = default;
	SkinMesh(const SkinMesh &other) = default;

	~SkinMesh() override = default;

	/**
	 * @brief Writes the SkinMesh object to the output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write the closing brace.
	 * @return The output stream after writing the SkinMesh object.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Updates the render object using the specified RendererObjectManager.
	 *
	 * @param manager The RendererObjectManager used to update the render object.
	 */
	void updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) override;

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

} // namespace Stone::Scene
