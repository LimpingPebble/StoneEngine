// Copyright 2024 Stone-Engine

#pragma once

#include <glm/mat4x4.hpp>
#include <memory>

namespace Stone::Scene {

class ISceneRenderer;

/**
 * @brief Represents the rendering context for the current frame of a scene.
 *
 * The RenderContext struct holds the necessary matrices and renderer for rendering a scene.
 * Each frame, the scene will create a RenderContext object and pass it to the root node of the scene.
 * The root node will then pass the RenderContext object to its children, and so on, until all nodes have been rendered.
 * It is passed as a modifiable reference to allow nodes to modify the matrices as needed.
 *
 * The renderer will create an inherited structure containing additional informations and send it to the
 * Node::render method of the root node.
 */
struct RenderContext {
	glm::mat4 projectionMatrix = glm::mat4(1.0f); /**< The projection matrix of the camera used for rendering. */
	glm::mat4 viewMatrix = glm::mat4(1.0f);		  /**< The view matrix of the camera used for rendering. */
	glm::mat4 modelMatrix = glm::mat4(1.0f);	  /**< The parent node world position matrix to use. */

	std::shared_ptr<ISceneRenderer> renderer;

	virtual ~RenderContext() = default; // Virtual destructor to allow inheritance
};

} // namespace Stone::Scene
