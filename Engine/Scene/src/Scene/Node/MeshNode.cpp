// Copyright 2024 Stone-Engine

#include "Scene/Node/MeshNode.hpp"

#include "Scene/ISceneRenderer.hpp"
#include "Scene/RenderElement/Material.hpp"
#include "Scene/RenderElement/Mesh.hpp"

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(MeshNode)

MeshNode::MeshNode(const std::string &name) : PivotNode(name), _mesh(nullptr), _material(nullptr) {
}

std::ostream &MeshNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	PivotNode::writeToStream(stream, false);
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
}

std::shared_ptr<Material> MeshNode::getMaterial() const {
	return _material;
}

void MeshNode::setMaterial(std::shared_ptr<Material> material) {
	_material = std::move(material);
}

const char *MeshNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_GREEN;
}

STONE_NODE_IMPLEMENTATION(InstancedMeshNode)

InstancedMeshNode::InstancedMeshNode(const std::string &name) : MeshNode(name), _instancesTransforms() {
}

std::ostream &InstancedMeshNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	MeshNode::writeToStream(stream, false);
	stream << ",instances:" << _instancesTransforms.size();
	if (closing_bracer)
		stream << "}";
	return stream;
}

void InstancedMeshNode::addInstance(const Transform3D &transform) {
	_instancesTransforms.push_back(transform);
}

void InstancedMeshNode::removeInstance(int index) {
	assert(index < static_cast<int>(_instancesTransforms.size()));
	_instancesTransforms.erase(_instancesTransforms.begin() + index);
}

void InstancedMeshNode::clearInstances() {
	_instancesTransforms.clear();
}

const std::vector<Transform3D> &InstancedMeshNode::getInstancesTransforms() const {
	return _instancesTransforms;
}

} // namespace Stone::Scene
