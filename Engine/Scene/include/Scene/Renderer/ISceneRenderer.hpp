// Copyright 2024 Stone-Engine

#pragma once

#include <memory>

namespace Stone::Scene {

class Node;
class WorldNode;

/**
 * @brief Interface for a scene renderer.
 */
class ISceneRenderer {
public:
	/**
	 * @brief Request the renderer to update all rendering data in the world.
	 */
	virtual void updateRenderablesInNode(const std::shared_ptr<Node> &rootNode) = 0;

	/**
	 * @brief Request the renderer to render the world from the given world root node.
	 */
	virtual void renderWorld(const std::shared_ptr<WorldNode> &world) = 0;
};

} // namespace Stone::Scene
