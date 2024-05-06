// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/RenderableNode.hpp"

namespace Stone::Scene {

class Mesh;
class Material;

class MeshNode : public RenderableNode {
public:
	STONE_NODE(MeshNode);

	explicit MeshNode(const std::string &name = "mesh");
	MeshNode(const MeshNode &other) = default;

	~MeshNode() override = default;

	void updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] std::shared_ptr<Mesh> getMesh() const;
	void setMesh(std::shared_ptr<Mesh> mesh);

	[[nodiscard]] std::shared_ptr<Material> getMaterial() const;
	void setMaterial(std::shared_ptr<Material> material);

protected:
	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Material> _material;

	[[nodiscard]] const char *_termClassColor() const override;
};

} // namespace Stone::Scene
