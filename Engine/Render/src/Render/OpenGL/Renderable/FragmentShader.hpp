// Copyright 2024 Stone-Engine

#pragma once

#include "../GlElements/ShaderPrograms.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Shader.hpp"

namespace Stone::Render::OpenGL {

class FragmentShader : public Scene::IRendererObject {
public:
	FragmentShader(Scene::FragmentShader &fragmentShader, const std::shared_ptr<OpenGLRenderer> &renderer) {
		_shaderPrograms = std::make_shared<ShaderPrograms>(fragmentShader, renderer->getOpenGLResources());
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

	const std::shared_ptr<ShaderPrograms> &getShaderPrograms() const {
		return _shaderPrograms;
	}

private:
	std::shared_ptr<ShaderPrograms> _shaderPrograms;
};

} // namespace Stone::Render::OpenGL
