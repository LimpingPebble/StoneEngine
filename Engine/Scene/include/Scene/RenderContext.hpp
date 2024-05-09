// Copyright 2024 Stone-Engine

#pragma once

#include <glm/mat4x4.hpp>
#include <memory>

namespace Stone::Scene {

class ISceneRenderer;

/**
 * @brief RenderContext is a struct that contains the information needed to render a scene.
 * The renderer will create an inherited structure to additional necassary informations.
 * The instance will be created by the renderer and passed to the Node::render method of the root node.
 */
struct RenderContext {
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	std::shared_ptr<ISceneRenderer> renderer;

	virtual ~RenderContext() = default; // Virtual destructor to allow inheritance
};

} // namespace Stone::Scene
