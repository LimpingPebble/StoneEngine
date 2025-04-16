// Copyright 2024 Stone-Engine

#pragma once

#include "../GlElements/ShaderPrograms.hpp"
#include "IOpenGLRendererObject.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Shader.hpp"

namespace Stone::Render::OpenGL {

class FragmentShader : public IOpenGLRendererObject {
public:
	FragmentShader(Scene::FragmentShader &fragmentShader, const std::shared_ptr<OpenGLRenderer> &renderer)
		: IOpenGLRendererObject(renderer) {
		_shaderPrograms = std::make_shared<ShaderPrograms>(fragmentShader, renderer->getResources());
	}

	void render(Scene::RenderContext &) override {
	}

	const std::shared_ptr<ShaderPrograms> &getShaderPrograms() const {
		return _shaderPrograms;
	}

private:
	std::shared_ptr<ShaderPrograms> _shaderPrograms;
};

} // namespace Stone::Render::OpenGL
