// Copyright 2024 Stone-Engine

#include "Scene/Node/CameraNode.hpp"

namespace Stone::Scene {

STONE_ABSTRACT_NODE_IMPLEMENTATION(CameraNode)

CameraNode::CameraNode(const std::string &name) : PivotNode(name), _near(0.1f), _far(100.0f) {
}

std::ostream &CameraNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	PivotNode::writeToStream(stream, false);
	stream << ",near:" << _near;
	stream << ",far:" << _far;
	if (closing_bracer)
		stream << "}";
	return stream;
}

float CameraNode::getNear() const {
	return _near;
}

void CameraNode::setNear(float near) {
	_near = near;
}

float CameraNode::getFar() const {
	return _far;
}

void CameraNode::setFar(float far) {
	_far = far;
}

const char *CameraNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_BLUE;
}

STONE_NODE_IMPLEMENTATION(PerspectiveCameraNode)

PerspectiveCameraNode::PerspectiveCameraNode(const std::string &name)
	: CameraNode(name), _fov(glm::radians(45.0f)), _aspect(1.0f) {
}

std::ostream &PerspectiveCameraNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	CameraNode::writeToStream(stream, false);
	stream << ",fov:" << _fov;
	stream << ",aspect:" << _aspect;
	if (closing_bracer)
		stream << "}";
	return stream;
}

glm::mat4 PerspectiveCameraNode::getProjectionMatrix() const {
	return glm::perspective(_fov, _aspect, _near, _far);
}

float PerspectiveCameraNode::getFov() const {
	return _fov;
}

void PerspectiveCameraNode::setFov(float fov) {
	_fov = fov;
}

float PerspectiveCameraNode::getAspect() const {
	return _aspect;
}

void PerspectiveCameraNode::setAspect(float aspect) {
	_aspect = aspect;
}

STONE_NODE_IMPLEMENTATION(OrthographicCameraNode)

OrthographicCameraNode::OrthographicCameraNode(const std::string &name) : CameraNode(name), _size(10.0f) {
}

std::ostream &OrthographicCameraNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	CameraNode::writeToStream(stream, false);
	stream << ",size:" << _size;
	if (closing_bracer)
		stream << "}";
	return stream;
}

glm::mat4 OrthographicCameraNode::getProjectionMatrix() const {
	return glm::ortho(-_size.x / 2, _size.x / 2, -_size.y / 2, _size.y / 2, _near, _far);
}

} // namespace Stone::Scene
