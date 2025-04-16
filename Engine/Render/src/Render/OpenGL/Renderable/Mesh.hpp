// Copyright 2024 Stone-Engine

#pragma once

#include "../GlElements/VRAMMesh.hpp"
#include "IOpenGLRendererObject.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Mesh.hpp"

namespace Stone::Render::OpenGL {

class RendererMesh : public IOpenGLRendererObject {

public:
	RendererMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: IOpenGLRendererObject(renderer), _vramMesh(mesh) {
	}

	~RendererMesh() override = default;

	const VRAMMesh &getVRAMMesh() const {
		return _vramMesh;
	}

private:
	VRAMMesh _vramMesh;
};


class DynamicMesh : public RendererMesh {
public:
	DynamicMesh(Scene::DynamicMesh &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: RendererMesh(std::static_pointer_cast<Scene::DynamicMesh>(mesh.shared_from_this()), renderer), _mesh(mesh) {
	}

	~DynamicMesh() override = default;

	void render(Scene::RenderContext &) override {
	}

private:
	Scene::DynamicMesh &_mesh;
};

class StaticMesh : public RendererMesh {
public:
	StaticMesh(Scene::StaticMesh &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: RendererMesh(mesh.getSourceMesh(), renderer), _mesh(mesh) {
	}

	~StaticMesh() override = default;

	void render(Scene::RenderContext &) override {
	}

private:
	Scene::StaticMesh &_mesh;
};

} // namespace Stone::Render::OpenGL
