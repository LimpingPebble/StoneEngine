// Copyright 2024 Stone-Engine

#include "Scene/Renderable/SkinMesh.hpp"

#include "Scene/Node/SkeletonNode.hpp"
#include "Scene/RendererObjectManager.hpp"

namespace Stone::Scene {

std::ostream &DynamicSkinMesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",vertices:" << _vertices.size();
	stream << ",indices:" << _indices.size();
	if (closing_bracer)
		stream << "}";
	return stream;
}

void DynamicSkinMesh::updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) {
	manager->updateDynamicSkinMesh(std::static_pointer_cast<DynamicSkinMesh>(shared_from_this()));
}

const std::vector<WeightVertex> &DynamicSkinMesh::getVertices() const {
	return _vertices;
}

std::vector<WeightVertex> &DynamicSkinMesh::verticesRef() {
	markDirty();
	return _vertices;
}

const std::vector<uint32_t> &DynamicSkinMesh::getIndices() const {
	return _indices;
}

std::vector<uint32_t> &DynamicSkinMesh::indicesRef() {
	markDirty();
	return _indices;
}

} // namespace Stone::Scene
