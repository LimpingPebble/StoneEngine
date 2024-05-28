// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/Renderable/IRenderable.hpp"
#include "Scene/Vertex.hpp"

#include <vector>

/**
 * @brief Represents a mesh used for rendering in the scene.
 *
 * It provides functionality for managing vertices and indices of the mesh.
 */
namespace Stone::Scene {

class Mesh : public Core::Object, public IRenderable {
public:
	Mesh() = default;
	Mesh(const Mesh &other) = default;

	~Mesh() override = default;

	/**
	 * @brief Writes the mesh data to the given output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write a closing bracer after the mesh data.
	 * @return The modified output stream.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Updates the render object associated with the mesh.
	 *
	 * @param manager The renderer object manager used to update the render object.
	 */
	void updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) override;

	/**
	 * @brief Retrieves the vertices of the mesh.
	 *
	 * @return A constant reference to the vector of vertices.
	 */
	[[nodiscard]] const std::vector<Vertex> &getVertices() const;

	/**
	 * @brief Retrieves the indices of the mesh.
	 *
	 * @return A constant reference to the vector of indices.
	 */
	[[nodiscard]] const std::vector<uint32_t> &getIndices() const;

	/**
	 * @brief Retrieves a reference to the vector of vertices.
	 *
	 * @note Using this method marks the mesh as dirty.
	 *
	 * @return A reference to the vector of vertices.
	 */
	std::vector<Vertex> &verticesRef();

	/**
	 * @brief Retrieves a reference to the vector of indices.
	 *
	 * @note Using this method marks the mesh as dirty.
	 *
	 * @return A reference to the vector of indices.
	 */
	std::vector<uint32_t> &indicesRef();

protected:
	std::vector<Vertex> _vertices;	/**< The vector of vertices. */
	std::vector<uint32_t> _indices; /**< The vector of indices. */
};

} // namespace Stone::Scene
