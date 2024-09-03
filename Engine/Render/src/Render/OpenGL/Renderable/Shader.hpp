// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Shader.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class Shader : public Scene::IRendererObject {
public:
	Shader(const std::shared_ptr<Scene::Shader> &shader, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _shader(shader), _renderer(renderer) {
	}

	~Shader() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	std::weak_ptr<Scene::Shader> _shader;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
