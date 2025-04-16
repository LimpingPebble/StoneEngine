// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/MeshNode.hpp"

namespace Stone::Render::OpenGL {

class Material;
class OpenGLRenderer;
class ShaderPrograms;

class MeshNode : public Scene::IRendererObject {
public:
	MeshNode(Scene::MeshNode &meshNode, const std::shared_ptr<OpenGLRenderer> &renderer);

	~MeshNode() override = default;

	void render(Scene::RenderContext &context) override;

private:
	Scene::MeshNode &_meshNode;
	std::weak_ptr<OpenGLRenderer> _renderer;
	std::shared_ptr<Material> _material;
	std::shared_ptr<ShaderPrograms> _shaderPrograms;
};

} // namespace Stone::Render::OpenGL
