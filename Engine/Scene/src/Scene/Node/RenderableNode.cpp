// Copyright 2024 Stone-Engine

#include "Scene/Node/RenderableNode.hpp"

namespace Stone::Scene {

STONE_ABSTRACT_NODE_IMPLEMENTATION(RenderableNode)

RenderableNode::RenderableNode(const std::string &name) : Node(name), IRenderable() {
}

void RenderableNode::render(RenderContext &context) {
	if (_rendererObject) {
		_rendererObject->render(context);
	}
	Node::render(context);
}

// TODO: Benchmark diamond inheritance with PivotNode vs pivot usage

} // namespace Stone::Scene
