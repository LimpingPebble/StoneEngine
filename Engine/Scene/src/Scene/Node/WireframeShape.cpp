// Copyright 2024 Stone-Engine

#include "Scene/Node/WireframeShape.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(WireframeShape)

WireframeShape::WireframeShape(const std::string &name)
	: RenderableNode(name), _color(glm::vec3(1.0f)), _thickness(1.0f), _points(), _drawLine(true), _lifespan(0.0f) {
}

std::ostream &WireframeShape::writeToStream(std::ostream &stream, bool closing_bracer) const {
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

void WireframeShape::update(float deltaTime) {
	RenderableNode::update(deltaTime);

	if (_lifespan > 0.0f) {
		_lifespan -= deltaTime;
		if (_lifespan <= 0.0f) {
			removeFromParent();
		}
	}
}

glm::vec3 WireframeShape::getColor() const {
	return _color;
}

void WireframeShape::setColor(const glm::vec3 &color) {
	_color = color;
	markDirty();
}

float WireframeShape::getThickness() const {
	return _thickness;
}

void WireframeShape::setThickness(float thickness) {
	_thickness = thickness;
	markDirty();
}

const std::vector<std::vector<glm::vec3>> &WireframeShape::getPoints() const {
	return _points;
}

std::vector<std::vector<glm::vec3>> &WireframeShape::pointsRef() {
	markDirty();
	return _points;
}

float WireframeShape::getLifespan() const {
	return _lifespan;
}

void WireframeShape::setLifespan(float lifespan) {
	_lifespan = lifespan;
}

}; // namespace Stone::Scene
