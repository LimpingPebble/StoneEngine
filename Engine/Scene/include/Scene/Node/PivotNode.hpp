// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/Node.hpp"
#include "Scene/Transform.hpp"

namespace Stone::Scene {

class PivotNode : public Node {
public:
	STONE_NODE(PivotNode);

	explicit PivotNode(const std::string &name = "pivot");
	PivotNode(const PivotNode &other);

	virtual ~PivotNode();

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	void render(RenderContext &context) override;

	void transformRelativeMatrix(glm::mat4 &relative) const override;

	Transform3D &getTransform();
	[[nodiscard]] const Transform3D &getTransform() const;
	void setTransform(const Transform3D &transform);

	const glm::mat4 &getTransformMatrix();
	[[nodiscard]] const glm::mat4 getTransformMatrix() const;

protected:
	Transform3D _transform;

	[[nodiscard]] const char *_termClassColor() const override;
};

} // namespace Stone::Scene
