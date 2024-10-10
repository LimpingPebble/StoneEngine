// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/DebugShape.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(DebugShape)

DebugShape::DebugShape(const std::string &name)
	: RenderableNode(name), _color(glm::vec3(1.0f)), _width(1.0f), _vertices(), _lifespan(0.0f) {
}

std::ostream &DebugShape::writeToStream(std::ostream &stream, bool closing_bracer) const {
	RenderableNode::writeToStream(stream, false);
	stream << ",color:" << _color;
	stream << ",width:" << _width;
	stream << ",vertices:[";
	for (std::size_t i = 0; i < _vertices.size(); ++i) {
		stream << _vertices[i];
		if (i < _vertices.size() - 1) {
			stream << ",";
		}
	}
	stream << "]";
	stream << ",lifespan:" << _lifespan;
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

float DebugShape::getWidth() const {
	return _width;
}

void DebugShape::setWidth(float width) {
	_width = width;
	markDirty();
}

const std::vector<glm::vec3> &DebugShape::getVertices() const {
	return _vertices;
}

std::vector<glm::vec3> &DebugShape::getVerticesRef() {
	markDirty();
	return _vertices;
}

float DebugShape::getLifespan() const {
	return _lifespan;
}

void DebugShape::setLifespan(float lifespan) {
	_lifespan = lifespan;
	markDirty();
}

}; // namespace Stone::Scene
