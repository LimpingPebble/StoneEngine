// Copyright 2024 Stone-Engine

#pragma once

#include "GlShaders.hpp"
#include "Scene/Renderable/IMeshObject.hpp"
#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/Shader.hpp"

namespace Stone::Render::OpenGL {

class OpenGLResources;

class ShaderPrograms {
public:
	ShaderPrograms(Scene::MaterialInputSignature params, const std::shared_ptr<OpenGLResources> &resources);
	ShaderPrograms(Scene::FragmentShader &shader, const std::shared_ptr<OpenGLResources> &resources);
	ShaderPrograms(Scene::Material &material, const std::shared_ptr<OpenGLResources> &resources);

	void makeMeshProgram();
	void makeSkinMeshProgram();
	void makeInstancedMeshProgram();
	void makeProgram(Scene::MeshType meshType);

	const std::unique_ptr<GlShaderProgram> &getMeshProgram() const;
	const std::unique_ptr<GlShaderProgram> &getSkinMeshProgram() const;
	const std::unique_ptr<GlShaderProgram> &getInstancedMeshProgram() const;
	GlShaderProgram *getProgram(Scene::MeshType meshType) const;

protected:
	std::weak_ptr<OpenGLResources> _resources;

	std::unique_ptr<GlShaderProgram> _meshProgram = nullptr;
	std::unique_ptr<GlShaderProgram> _skinMeshProgram = nullptr;
	std::unique_ptr<GlShaderProgram> _instancedMeshProgram = nullptr;

	std::unique_ptr<GlFragmentShader> _glFragmentShader;
};

} // namespace Stone::Render::OpenGL
