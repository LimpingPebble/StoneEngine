// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/RenderableNode.hpp"

namespace Stone::Scene {

/**
 * @class DebugShape
 * @brief Represents a debug shape node.
 *
 * The `DebugShape` class represents a debug shape node in the scene graph.
 * It's transform location in the scene graph does not affect the rendering of the shape.
 */
class DebugShape : public RenderableNode {
	STONE_NODE(DebugShape);

public:
	explicit DebugShape(const std::string &name = "debug_shape");
	DebugShape(const DebugShape &other) = default;

	~DebugShape() override = default;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	void update(float deltaTime) override;

	[[nodiscard]] glm::vec3 getColor() const;
	void setColor(const glm::vec3 &color);

	[[nodiscard]] float getWidth() const;
	void setWidth(float width);

	[[nodiscard]] const std::vector<glm::vec3> &getVertices() const;
	std::vector<glm::vec3> &getVerticesRef();

	[[nodiscard]] float getLifespan() const;
	void setLifespan(float lifespan);

protected:
	glm::vec3 _color;				  /** The color of the debug shape. */
	float _width;					  /** The line width of the debug shape. */
	std::vector<glm::vec3> _vertices; /** The vertices of the debug shape. */

	float _lifespan; /** The lifespan of the debug shape. */
};

} // namespace Stone::Scene
