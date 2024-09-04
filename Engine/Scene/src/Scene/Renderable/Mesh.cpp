// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Mesh.hpp"

namespace Stone::Scene {

std::ostream &DynamicMesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",vertices:" << _vertices.size();
	stream << ",indices:" << _indices.size();
	if (closing_bracer)
		stream << "}";
	return stream;
}

const std::vector<Vertex> &DynamicMesh::getVertices() const {
	return _vertices;
}

const std::vector<uint32_t> &DynamicMesh::getIndices() const {
	return _indices;
}

std::vector<Vertex> &DynamicMesh::verticesRef() {
	markDirty();
	return _vertices;
}

std::vector<uint32_t> &DynamicMesh::indicesRef() {
	markDirty();
	return _indices;
}


std::ostream &StaticMesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",dynamic_mesh:" << _dynamicMesh ? std::to_string(_dynamicMesh->getId()) : "null";
	if (closing_bracer)
		stream << "}";
	return stream;
}

const std::shared_ptr<DynamicMesh> &StaticMesh::getSourceMesh() const {
	return _dynamicMesh;
}

void StaticMesh::setSourceMesh(const std::shared_ptr<DynamicMesh> &sourceMesh) {
	_dynamicMesh = sourceMesh;
	markDirty();
}


} // namespace Stone::Scene
