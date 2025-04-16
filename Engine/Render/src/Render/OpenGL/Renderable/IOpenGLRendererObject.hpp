// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderable/IRenderable.hpp"

namespace Stone::Render::OpenGL {

class OpenGLRenderer;

class IOpenGLRendererObject : public Scene::IRendererObject {
public:
	IOpenGLRendererObject(const std::shared_ptr<OpenGLRenderer> &renderer) : _renderer(renderer) {
	}

	~IOpenGLRendererObject() override = default;

protected:
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
