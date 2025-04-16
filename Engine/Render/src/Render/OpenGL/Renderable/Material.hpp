// Copyright 2024 Stone-Engine

#pragma once

#include "IOpenGLRendererObject.hpp"
#include "Scene/Renderable/IMeshObject.hpp"
#include "Scene/Renderable/Material.hpp"


namespace Stone::Render::OpenGL {

class ShaderPrograms;

class Material : public IOpenGLRendererObject {
public:
	Material(Scene::Material &material, const std::shared_ptr<OpenGLRenderer> &renderer);

	~Material() override = default;

	void render(Scene::RenderContext &) override {
	}

	void setUniforms(Scene::MeshType meshType);

	const std::shared_ptr<ShaderPrograms> &getShaderPrograms() const;

private:
	Scene::Material &_material;
	std::shared_ptr<ShaderPrograms> _shaderPrograms;
};

} // namespace Stone::Render::OpenGL
