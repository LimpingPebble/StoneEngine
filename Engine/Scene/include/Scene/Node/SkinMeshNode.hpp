// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene {

class SkinMesh;
class SkeletonNode;
class Material;

class SkinMeshNode : public PivotNode {
public:
	STONE_NODE(SkinMeshNode);

	explicit SkinMeshNode(const std::string &name = "skinmesh");
	SkinMeshNode(const SkinMeshNode &other);

	virtual ~SkinMeshNode();

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] std::shared_ptr<SkinMesh> getSkinMesh() const;
	void setSkinMesh(std::shared_ptr<SkinMesh> mesh);

	[[nodiscard]] std::shared_ptr<Material> getMaterial() const;
	void setMaterial(std::shared_ptr<Material> material);

	[[nodiscard]] std::shared_ptr<SkeletonNode> getSkeleton() const;
	void setSkeleton(std::shared_ptr<SkeletonNode> skeleton);

protected:
	std::shared_ptr<SkinMesh> _mesh;
	std::shared_ptr<Material> _material;
	std::weak_ptr<SkeletonNode> _skeleton;

	[[nodiscard]] const char *_termClassColor() const override;
};

} // namespace Stone::Scene
