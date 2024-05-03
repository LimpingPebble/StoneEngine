// Copyright 2024 Stone-Engine

#include "Scene/Node/WorldNode.hpp"

#include "Scene/Node/CameraNode.hpp"

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(WorldNode)

WorldNode::WorldNode(const std::string &name) : Node(name), _activeCamera() {
}

std::ostream &WorldNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Node::writeToStream(stream, false);
	stream << ",active_camera:" << (_activeCamera.expired() ? "null" : _activeCamera.lock()->getGlobalName());
	if (closing_bracer)
		stream << "}";
	return stream;
}

void WorldNode::setRenderer(const std::shared_ptr<ISceneRenderer> &renderer) {
	_renderer = renderer;
}

std::shared_ptr<ISceneRenderer> WorldNode::getRenderer() const {
	return _renderer;
}

void WorldNode::setActiveCamera(const std::shared_ptr<CameraNode> &camera) {
	_activeCamera = camera;
}

std::shared_ptr<CameraNode> WorldNode::getActiveCamera() const {
	return _activeCamera.lock();
}

void WorldNode::render() {
	RenderContext context;

	if (auto camera = _activeCamera.lock()) {
		context.viewMatrix = glm::inverse(camera->getWorldTransformMatrix());
		context.projectionMatrix = camera->getProjectionMatrix();
	}

	Node::render(context);
}

const char *WorldNode::_termClassColor() const {
	return TERM_COLOR_RED;
}

} // namespace Stone::Scene
