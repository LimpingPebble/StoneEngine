// Copyright 2024 Stone-Engine

#include "Scene/Node/WorldNode.hpp"

#include "Scene/Node/CameraNode.hpp"

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(WorldNode)

std::shared_ptr<WorldNode> WorldNode::create() {
	auto new_world = std::make_shared<WorldNode>();
	new_world->_world = new_world;
	return new_world;
}

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

void WorldNode::initializeRenderContext(RenderContext &context) const {
	if (auto camera = _activeCamera.lock()) {
		context.mvp.viewMatrix = glm::inverse(camera->getWorldTransformMatrix());
		context.mvp.projMatrix = camera->getProjectionMatrix();
	}
}

const char *WorldNode::_termClassColor() const {
	return TERM_COLOR_RED;
}

} // namespace Stone::Scene
