// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Node/MeshNode.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class MeshNode : public Scene::IRendererObject {
public:
	MeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _meshNode(meshNode), _renderer(renderer) {
	}

	~MeshNode() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	std::weak_ptr<Scene::MeshNode> _meshNode;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
