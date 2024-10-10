// Copyright 2024 Stone-Engine

#include "Scene/Node/DebugShape.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(DebugShape)

DebugShape::DebugShape(const std::string &name)
	: RenderableNode(name), _color(glm::vec3(1.0f)), _thickness(1.0f), _points(), _drawLine(true), _lifespan(0.0f) {
}

std::ostream &DebugShape::writeToStream(std::ostream &stream, bool closing_bracer) const {
	RenderableNode::writeToStream(stream, false);
	stream << ",color:" << _color;
	stream << ",thickness:" << _thickness;
	stream << ",points:[";
	for (std::size_t i = 0; i < _points.size(); i++) {
		if (i > 0)
			stream << ",";
		stream << "[";
		for (std::size_t j = 0; j < _points[i].size(); j++) {
			if (j > 0)
				stream << ",";
			stream << _points[i][j];
		}
		stream << "]";
	}
	stream << "],lifespan:" << _lifespan;
	if (closing_bracer)
		stream << "}";
	return stream;
}

void DebugShape::update(float deltaTime) {
	RenderableNode::update(deltaTime);

	if (_lifespan > 0.0f) {
		_lifespan -= deltaTime;
		if (_lifespan <= 0.0f) {
			removeFromParent();
		}
	}
}

glm::vec3 DebugShape::getColor() const {
	return _color;
}

void DebugShape::setColor(const glm::vec3 &color) {
	_color = color;
	markDirty();
}

float DebugShape::getThickness() const {
	return _thickness;
}

void DebugShape::setThickness(float thickness) {
	_thickness = thickness;
	markDirty();
}

const std::vector<std::vector<glm::vec3>> &DebugShape::getPoints() const {
	return _points;
}

std::vector<std::vector<glm::vec3>> &DebugShape::pointsRef() {
	markDirty();
	return _points;
}

float DebugShape::getLifespan() const {
	return _lifespan;
}

void DebugShape::setLifespan(float lifespan) {
	_lifespan = lifespan;
}

}; // namespace Stone::Scene
