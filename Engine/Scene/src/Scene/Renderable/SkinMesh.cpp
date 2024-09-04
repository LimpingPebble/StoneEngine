// Copyright 2024 Stone-Engine

#include "Scene/Renderable/SkinMesh.hpp"

#include "Scene/Node/SkeletonNode.hpp"

namespace Stone::Scene {

std::ostream &DynamicSkinMesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",vertices:" << _vertices.size();
	stream << ",indices:" << _indices.size();
	if (closing_bracer)
		stream << "}";
	return stream;
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


std::ostream &StaticSkinMesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",source_mesh:" << (_dynamicMesh ? std::to_string(_dynamicMesh->getId()) : "null");
	if (closing_bracer)
		stream << "}";
	return stream;
}

const std::shared_ptr<DynamicSkinMesh> &StaticSkinMesh::getSourceMesh() const {
	return _dynamicMesh;
}

void StaticSkinMesh::setSourceMesh(const std::shared_ptr<DynamicSkinMesh> &sourceMesh) {
	_dynamicMesh = sourceMesh;
	markDirty();
}

} // namespace Stone::Scene
