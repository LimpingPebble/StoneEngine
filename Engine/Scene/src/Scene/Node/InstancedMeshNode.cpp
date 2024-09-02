// Copyright 2024 Stone-Engine

#include "Scene/Node/InstancedMeshNode.hpp"

#include "Scene/RendererObjectManager.hpp"

namespace Stone::Scene {

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
	markDirty();
}

void InstancedMeshNode::removeInstance(int index) {
	assert(index < static_cast<int>(_instancesTransforms.size()));
	_instancesTransforms.erase(_instancesTransforms.begin() + index);
	markDirty();
}

void InstancedMeshNode::clearInstances() {
	_instancesTransforms.clear();
	markDirty();
}

const std::vector<Transform3D> &InstancedMeshNode::getInstancesTransforms() const {
	return _instancesTransforms;
}

Transform3D &InstancedMeshNode::instanceTransformRef(size_t index) {
	assert(index < _instancesTransforms.size());
	markDirty();
	return _instancesTransforms[index];
}

} // namespace Stone::Scene
