// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/MeshNode.hpp"
#include "Scene/Transform.hpp"

namespace Stone::Scene {

class InstancedMeshNode : public MeshNode {
public:
	STONE_NODE(InstancedMeshNode);

	explicit InstancedMeshNode(const std::string &name = "instancedmesh");
	InstancedMeshNode(const InstancedMeshNode &other) = default;

	~InstancedMeshNode() override = default;

	void updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	void addInstance(const Transform3D &transform);
	void removeInstance(int index);
	void clearInstances();

	[[nodiscard]] const std::vector<Transform3D> &getInstancesTransforms() const;
	[[nodiscard]] Transform3D &instanceTransformRef(size_t index);

protected:
	std::vector<Transform3D> _instancesTransforms;
};

} // namespace Stone::Scene
