// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/SkinMesh.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class DynamicSkinMesh : public Scene::IRendererObject {
public:
	DynamicSkinMesh(const std::shared_ptr<Scene::DynamicSkinMesh> &skinMesh,
					const std::shared_ptr<OpenGLRenderer> &renderer)
		: _skinMesh(skinMesh), _renderer(renderer) {
	}

	~DynamicSkinMesh() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	std::weak_ptr<Scene::DynamicSkinMesh> _skinMesh;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

class StaticSkinMesh : public Scene::IRendererObject {
public:
	StaticSkinMesh(const std::shared_ptr<Scene::StaticSkinMesh> &skinMesh,
				   const std::shared_ptr<OpenGLRenderer> &renderer)
		: _skinMesh(skinMesh), _renderer(renderer) {
	}

	~StaticSkinMesh() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	std::weak_ptr<Scene::StaticSkinMesh> _skinMesh;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
