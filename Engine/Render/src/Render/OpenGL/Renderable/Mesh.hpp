// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Mesh.hpp"

#include "../GlElements/VramMesh.hpp"

namespace Stone::Render::OpenGL {

class RendererMesh : public Scene::IRendererObject {

public:
	RendererMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _vramMesh(mesh, renderer) {
	}

	~RendererMesh() override {
	}

	const VRAMMesh &getVRAMMesh() const {
		return _vramMesh;
	}

private:
	VRAMMesh _vramMesh;
};


class DynamicMesh : public RendererMesh {
public:
	DynamicMesh(Scene::DynamicMesh &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: RendererMesh(std::static_pointer_cast<Scene::DynamicMesh>(mesh.shared_from_this()), renderer), _mesh(mesh),
		  _renderer(renderer) {
	}

	~DynamicMesh() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	Scene::DynamicMesh &_mesh;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

class StaticMesh : public RendererMesh {
public:
	StaticMesh(Scene::StaticMesh &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: RendererMesh(mesh.getSourceMesh(), renderer), _mesh(mesh), _renderer(renderer) {
	}

	~StaticMesh() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	Scene::StaticMesh &_mesh;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
