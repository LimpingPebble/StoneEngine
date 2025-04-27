// Copyright 2024 Stone-Engine

#pragma once

#include "IOpenGLRendererObject.hpp"
#include "Scene/Node/SkinMeshNode.hpp"

namespace Stone::Render::OpenGL {

class SkinMeshNode : public IOpenGLRendererObject {
public:
	SkinMeshNode(Scene::SkinMeshNode &skinMeshNode, const std::shared_ptr<OpenGLRenderer> &renderer)
		: IOpenGLRendererObject(renderer), _skinMeshNode(skinMeshNode) {
	}

	~SkinMeshNode() override = default;

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	Scene::SkinMeshNode &_skinMeshNode;
};

} // namespace Stone::Render::OpenGL
