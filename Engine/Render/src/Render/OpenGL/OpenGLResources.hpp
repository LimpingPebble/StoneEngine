// Copyright 2024 Stone-Engine

#pragma once

#include "GlElements/GlShaders.hpp"
#include "GlElements/ShaderPrograms.hpp"
#include "Scene/Renderable/Material.hpp"

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

	const std::unique_ptr<GlFragmentShader> &getFragmentShader(Scene::MaterialInputSignature params);

	// MARK: Shader Programs

	const std::unique_ptr<ShaderPrograms> &getDefaultShaderPrograms();
	const std::unique_ptr<ShaderPrograms> &getStandardShaderPrograms(Scene::MaterialInputSignature params);

private:
	std::weak_ptr<OpenGLRenderer> _renderer;

	std::unique_ptr<GlVertexShader> _meshVertexShader;
	std::unique_ptr<GlVertexShader> _skinMeshVertexShader;
	std::unique_ptr<GlVertexShader> _instancedMeshVertexShader;

	std::unordered_map<Scene::MaterialInputSignature, std::unique_ptr<GlFragmentShader>> _fragmentShaders;

	std::unique_ptr<ShaderPrograms> _defaultShaderPrograms;
	std::unordered_map<Scene::MaterialInputSignature, std::unique_ptr<ShaderPrograms>> _standardsShaderPrograms;
};

} // namespace Stone::Render::OpenGL
