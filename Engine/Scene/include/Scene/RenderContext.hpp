// Copyright 2024 Stone-Engine

#pragma once

#include <glm/mat4x4.hpp>
#include <memory>

namespace Stone::Scene {

class ISceneRenderer;

struct RenderContext {
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	std::shared_ptr<ISceneRenderer> renderer;
};

} // namespace Stone::Scene
