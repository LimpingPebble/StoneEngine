// Copyright 2024 Stone-Engine

#include "Scene/Node/MeshNode.hpp"

#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/Mesh.hpp"
#include "Scene/RendererObjectManager.hpp"

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(MeshNode)

MeshNode::MeshNode(const std::string &name) : RenderableNode(name), _mesh(nullptr), _material(nullptr) {
}

void MeshNode::updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) {
	manager->updateMeshNode(std::dynamic_pointer_cast<MeshNode>(shared_from_this()));
}

std::ostream &MeshNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	RenderableNode::writeToStream(stream, false);
	stream << ",mesh:" << (_mesh ? std::to_string(_mesh->getId()) : "null");
	stream << ",material:" << (_material ? std::to_string(_material->getId()) : "null");
	if (closing_bracer)
		stream << "}";
	return stream;
}

std::shared_ptr<Mesh> MeshNode::getMesh() const {
	return _mesh;
}

void MeshNode::setMesh(std::shared_ptr<Mesh> mesh) {
	_mesh = std::move(mesh);
	markDirty();
}

std::shared_ptr<Material> MeshNode::getMaterial() const {
	return _material;
}

void MeshNode::setMaterial(std::shared_ptr<Material> material) {
	_material = std::move(material);
	markDirty();
}

const char *MeshNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_GREEN;
}

} // namespace Stone::Scene
