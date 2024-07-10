// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/Node.hpp"
#include "Scene/Renderable/IRenderable.hpp"

namespace Stone::Scene {

/**
 * @class RenderableNode
 * @brief Represents a node that can be rendered.
 */
class RenderableNode : public Node, public IRenderable {
public:
	STONE_ABSTRACT_NODE(RenderableNode)

	explicit RenderableNode(const std::string &name = "renderable");
	RenderableNode(const RenderableNode &other) = default;

	~RenderableNode() override = default;

	void render(RenderContext &context) override;
};

} // namespace Stone::Scene
