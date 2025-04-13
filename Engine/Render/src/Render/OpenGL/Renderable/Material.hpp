// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/IMeshObject.hpp"
#include "Scene/Renderable/Material.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class ShaderPrograms;

class Material : public Scene::IRendererObject {
public:
	Material(Scene::Material &material, const std::shared_ptr<OpenGLRenderer> &renderer);

	~Material() override = default;

	void render(Scene::RenderContext &context) override;

	void setUniforms(Scene::MeshType meshType);

	const std::shared_ptr<ShaderPrograms> &getShaderPrograms() const;

private:
	Scene::Material &_material;
	std::shared_ptr<ShaderPrograms> _shaderPrograms;
};

} // namespace Stone::Render::OpenGL
