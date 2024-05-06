// Copyright 2024 Stone-Engine

#include "Scene/RenderElement/SkinMesh.hpp"

#include "Scene/Node/SkeletonNode.hpp"
#include "Scene/RendererObjectManager.hpp"

namespace Stone::Scene {

std::ostream &SkinMesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",vertices:" << _vertices.size();
	stream << ",indices:" << _indices.size();
	if (closing_bracer)
		stream << "}";
	return stream;
}

void SkinMesh::updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) {
	manager->updateSkinMesh(std::static_pointer_cast<SkinMesh>(shared_from_this()));
}

const std::vector<WeightVertex> &SkinMesh::getVertices() const {
	return _vertices;
}

std::vector<WeightVertex> &SkinMesh::verticesRef() {
	markDirty();
	return _vertices;
}

const std::vector<uint32_t> &SkinMesh::getIndices() const {
	return _indices;
}

std::vector<uint32_t> &SkinMesh::indicesRef() {
	markDirty();
	return _indices;
}

} // namespace Stone::Scene
