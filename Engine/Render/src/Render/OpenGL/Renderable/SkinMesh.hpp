// Copyright 2024 Stone-Engine

#pragma once

#include "../GlElements/VRAMSkinMesh.hpp"
#include "IOpenGLRendererObject.hpp"
#include "Scene/Renderable/SkinMesh.hpp"

namespace Stone::Render::OpenGL {

class RendererSkinMesh : public IOpenGLRendererObject {
public:
	RendererSkinMesh(const std::shared_ptr<Scene::DynamicSkinMesh> &skinMesh,
					 const std::shared_ptr<OpenGLRenderer> &renderer)
		: IOpenGLRendererObject(renderer), _vramMesh(skinMesh) {
	}

	~RendererSkinMesh() override = default;

	const VRAMSkinMesh &getVRAMSkinMesh() const {
		return _vramMesh;
	}

private:
	VRAMSkinMesh _vramMesh;
};

class DynamicSkinMesh : public RendererSkinMesh {
public:
	DynamicSkinMesh(Scene::DynamicSkinMesh &skinMesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: RendererSkinMesh(std::static_pointer_cast<Scene::DynamicSkinMesh>(skinMesh.shared_from_this()), renderer),
		  _skinMesh(skinMesh) {
	}

	~DynamicSkinMesh() override = default;

	void render(Scene::RenderContext &) override {
	}

private:
	Scene::DynamicSkinMesh &_skinMesh;
};

class StaticSkinMesh : public RendererSkinMesh {
public:
	StaticSkinMesh(Scene::StaticSkinMesh &skinMesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: RendererSkinMesh(skinMesh.getSourceMesh(), renderer), _skinMesh(skinMesh) {
	}

	~StaticSkinMesh() override = default;

	void render(Scene::RenderContext &) override {
	}

private:
	Scene::StaticSkinMesh &_skinMesh;
};

} // namespace Stone::Render::OpenGL
