// Copyright 2024 Stone-Engine

#pragma once

#include "IOpenGLRendererObject.hpp"
#include "Scene/Node/InstancedMeshNode.hpp"

namespace Stone::Render::OpenGL {

class InstancedMeshNode : public IOpenGLRendererObject {
public:
	InstancedMeshNode(Scene::InstancedMeshNode &instancedMeshNode, const std::shared_ptr<OpenGLRenderer> &renderer)
		: IOpenGLRendererObject(renderer), _instancedMeshNode(instancedMeshNode) {
	}

	~InstancedMeshNode() override {
	}

	void render(Scene::RenderContext &) override {
	}

private:
	Scene::InstancedMeshNode &_instancedMeshNode;
};

} // namespace Stone::Render::OpenGL
