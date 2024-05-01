// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/RenderElement/IRenderElement.hpp"
#include "Scene/Vertex.hpp"

#include <vector>

namespace Stone::Scene {

class Mesh : public Core::Object, public IRenderElement {
public:
	Mesh() = default;
	Mesh(const Mesh &other) = default;

	~Mesh() override = default;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	void updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) override;

	[[nodiscard]] const std::vector<Vertex> &getVertices() const;
	[[nodiscard]] const std::vector<uint32_t> &getIndices() const;
	std::vector<Vertex> &verticesRef();
	std::vector<uint32_t> &indicesRef();

protected:
	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
};

} // namespace Stone::Scene
