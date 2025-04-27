// Copyright 2024 Stone-Engine

#pragma once

#include "IOpenGLRendererObject.hpp"
#include "Scene/Node/MeshNode.hpp"

namespace Stone::Render::OpenGL {

class Material;
class ShaderPrograms;

class MeshNode : public IOpenGLRendererObject {
public:
	MeshNode(Scene::MeshNode &meshNode, const std::shared_ptr<OpenGLRenderer> &renderer);

	~MeshNode() override = default;

	void render(Scene::RenderContext &context) override;

private:
	Scene::MeshNode &_meshNode;
	std::shared_ptr<Material> _material;
	std::shared_ptr<ShaderPrograms> _shaderPrograms;
};

} // namespace Stone::Render::OpenGL
