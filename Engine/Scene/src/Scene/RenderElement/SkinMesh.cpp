// Copyright 2024 Stone-Engine

#include "Scene/RenderElement/SkinMesh.hpp"

#include "Scene/ISceneRenderer.hpp"
#include "Scene/Node/SkeletonNode.hpp"

namespace Stone::Scene {

SkinMesh::SkinMesh() : Object(), IRenderElement(), _vertices(), _indices() {
}

SkinMesh::SkinMesh(const SkinMesh &other)
	: Object(other), IRenderElement(other), _vertices(other._vertices), _indices(other._indices) {
}

SkinMesh::~SkinMesh() {
}

std::ostream &SkinMesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",vertices:" << _vertices.size();
	stream << ",indices:" << _indices.size();
	if (closing_bracer)
		stream << "}";
	return stream;
}

void SkinMesh::generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer) {
	renderer->generateDataForSkinMesh(std::static_pointer_cast<SkinMesh>(shared_from_this()));
}

const std::vector<WeightVertex> &SkinMesh::getVertices() const {
	return _vertices;
}

std::vector<WeightVertex> &SkinMesh::verticesRef() {
	return _vertices;
}

const std::vector<uint32_t> &SkinMesh::getIndices() const {
	return _indices;
}

std::vector<uint32_t> &SkinMesh::indicesRef() {
	return _indices;
}

} // namespace Stone::Scene
