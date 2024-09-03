// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Material.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class Material : public Scene::IRendererObject {
public:
	Material(const std::shared_ptr<Scene::Material> &material, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _material(material), _renderer(renderer) {
	}

	~Material() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	std::weak_ptr<Scene::Material> _material;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
