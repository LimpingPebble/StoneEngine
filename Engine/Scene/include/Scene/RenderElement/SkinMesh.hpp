// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/RenderElement/IRenderElement.hpp"
#include "Scene/Vertex.hpp"

#include <vector>

namespace Stone::Scene {

class SkinMesh : public Core::Object, public IRenderElement {
public:
	SkinMesh();
	SkinMesh(const SkinMesh &other);

	virtual ~SkinMesh();

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	void generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer) override;

	[[nodiscard]] const std::vector<WeightVertex> &getVertices() const;
	std::vector<WeightVertex> &verticesRef();

	[[nodiscard]] const std::vector<uint32_t> &getIndices() const;
	std::vector<uint32_t> &indicesRef();

protected:
	std::vector<WeightVertex> _vertices;
	std::vector<uint32_t> _indices;
};

} // namespace Stone::Scene
