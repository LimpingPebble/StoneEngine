// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Mesh.hpp"

#include "Scene/RendererObjectManager.hpp"

namespace Stone::Scene {

std::ostream &Mesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",vertices:" << _vertices.size();
	stream << ",indices:" << _indices.size();
	if (closing_bracer)
		stream << "}";
	return stream;
}

void Mesh::updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) {
	manager->updateMesh(std::static_pointer_cast<Mesh>(shared_from_this()));
}

const std::vector<Vertex> &Mesh::getVertices() const {
	return _vertices;
}

const std::vector<uint32_t> &Mesh::getIndices() const {
	return _indices;
}

std::vector<Vertex> &Mesh::verticesRef() {
	markDirty();
	return _vertices;
}

std::vector<uint32_t> &Mesh::indicesRef() {
	markDirty();
	return _indices;
}

} // namespace Stone::Scene
