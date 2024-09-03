// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Mesh.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class DynamicMesh : public Scene::IRendererObject {
public:
	DynamicMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _mesh(mesh), _renderer(renderer) {
	}

	~DynamicMesh() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	std::weak_ptr<Scene::DynamicMesh> _mesh;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

class StaticMesh : public Scene::IRendererObject {
public:
	StaticMesh(const std::shared_ptr<Scene::StaticMesh> &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _mesh(mesh), _renderer(renderer) {
	}

	~StaticMesh() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	std::weak_ptr<Scene::StaticMesh> _mesh;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
