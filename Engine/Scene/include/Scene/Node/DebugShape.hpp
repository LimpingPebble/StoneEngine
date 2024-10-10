// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Geometry.hpp"
#include "Scene/Node/RenderableNode.hpp"

namespace Stone::Scene {

/**
 * @class DebugShape
 * @brief Represents a debug shape node.
 *
 * The `DebugShape` class represents a debug shape node in the scene graph.
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

	[[nodiscard]] float getThickness() const;
	void setThickness(float thickness);

	[[nodiscard]] const std::vector<std::vector<glm::vec3>> &getPoints() const;
	std::vector<std::vector<glm::vec3>> &pointsRef();

	[[nodiscard]] bool isDrawLine() const;
	void setDrawLine(bool drawLine);

	[[nodiscard]] float getLifespan() const;
	void setLifespan(float lifespan);

	template <typename ShapeType, typename... Args>
	static std::shared_ptr<DebugShape> create(const ShapeType &shape, Args &&...args) {
		auto debugShape = std::make_shared<DebugShape>();

		auto [indices, vertices] = generateGeometryMesh(shape, std::forward<Args>(args)...);

		auto &points = debugShape->pointsRef().emplace_back();
		points.reserve(indices.size());
		for (auto &index : indices) {
			points.push_back(vertices[index]);
		}

		return debugShape;
	}

protected:
	glm::vec3 _color; /** The color of the debug shape. */
	float _thickness; /** The line thickness of the debug shape. */

	std::vector<std::vector<glm::vec3>> _points; /** The vertices of the debug shape. */

	bool _drawLine; /** Whether to draw the debug shape as a line. */

	float _lifespan; /** The lifespan of the debug shape. */
};

} // namespace Stone::Scene
