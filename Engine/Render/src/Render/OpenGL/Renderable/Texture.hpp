// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Texture.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class Texture : public Scene::IRendererObject {
public:
	Texture(const std::shared_ptr<Scene::Texture> &texture, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _texture(texture), _renderer(renderer) {
	}

	~Texture() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	std::weak_ptr<Scene::Texture> _texture;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
