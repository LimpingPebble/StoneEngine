// Copyright 2024 Stone-Engine

#pragma once

#include "GlElements/GlShaders.hpp"
#include "GlElements/ShaderPrograms.hpp"
#include "Scene/Shader/ShaderParameters.hpp"

namespace Stone::Render::OpenGL {

class OpenGLRenderer;

class OpenGLResources : std::enable_shared_from_this<OpenGLResources> {
public:
	OpenGLResources() = delete;

	OpenGLResources(const std::shared_ptr<OpenGLRenderer> &renderer);

	virtual ~OpenGLResources() = default;

	const std::weak_ptr<OpenGLRenderer> getRenderer() const;

	// MARK: Vertex Shaders

	const std::unique_ptr<GlVertexShader> &getMeshVertexShader();
	const std::unique_ptr<GlVertexShader> &getSkinMeshVertexShader();
	const std::unique_ptr<GlVertexShader> &getInstancedMeshVertexShader();
	const std::unique_ptr<GlVertexShader> &getVertexShader(Scene::MeshType meshType);

	// MARK: Fragment Shaders

	const std::unique_ptr<GlFragmentShader> &getFragmentShader(Scene::ShaderParameters params);

	// MARK: Shader Programs

	const std::unique_ptr<ShaderPrograms> &getDefaultShaderPrograms();

private:
	std::weak_ptr<OpenGLRenderer> _renderer;

	std::unique_ptr<GlVertexShader> _meshVertexShader;
	std::unique_ptr<GlVertexShader> _skinMeshVertexShader;
	std::unique_ptr<GlVertexShader> _instancedMeshVertexShader;

	std::unordered_map<Scene::ShaderParameters, std::unique_ptr<GlFragmentShader>> _fragmentShaders;

	std::unique_ptr<ShaderPrograms> _defaultShaderPrograms;
};

} // namespace Stone::Render::OpenGL
