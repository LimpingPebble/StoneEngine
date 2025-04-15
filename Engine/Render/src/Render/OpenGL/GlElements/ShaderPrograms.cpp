// Copyright 2024 Stone-Engine

#include "ShaderPrograms.hpp"

#include "../OpenGLDirector.hpp"
#include "../OpenGLResources.hpp"
#include "GlShaders.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"

namespace Stone::Render::OpenGL {

ShaderPrograms::ShaderPrograms(Scene::MaterialInputSignature params, const std::shared_ptr<OpenGLResources> &resources) : _resources(resources) {
	switch (resources->getRenderer().lock()->getDirector()->getRenderingMethod()) {
	case RenderingMethod::Forward: _glFragmentShader = GlFragmentShader::makeStandardForwardShader(params); break;
	case RenderingMethod::Deferred: _glFragmentShader = GlFragmentShader::makeStandardDeferredShader(params); break;
	}
}

ShaderPrograms::ShaderPrograms(Scene::FragmentShader &shader, const std::shared_ptr<OpenGLResources> &resources)
	: _resources(resources) {
	_glFragmentShader = std::make_unique<GlFragmentShader>(shader);
}

ShaderPrograms::ShaderPrograms(Scene::Material &material, const std::shared_ptr<OpenGLResources> &resources)
	: _resources(resources) {
	Scene::MaterialInputSignature params(std::dynamic_pointer_cast<Scene::Material>(material.shared_from_this()));
	switch (resources->getRenderer().lock()->getDirector()->getRenderingMethod()) {
	case RenderingMethod::Forward: _glFragmentShader = GlFragmentShader::makeStandardForwardShader(params); break;
	case RenderingMethod::Deferred: _glFragmentShader = GlFragmentShader::makeStandardDeferredShader(params); break;
	}
}

void ShaderPrograms::makeMeshProgram() {
	if (_meshProgram != nullptr)
		return;

	assert(_resources.expired() == false);

	auto &vertexShader = _resources.lock()->getMeshVertexShader();
	assert(vertexShader != nullptr);

	_meshProgram = std::make_unique<GlShaderProgram>(*vertexShader, *_glFragmentShader);
}

void ShaderPrograms::makeSkinMeshProgram() {
	if (_skinMeshProgram != nullptr)
		return;

	assert(_resources.expired() == false);

	auto &vertexShader = _resources.lock()->getSkinMeshVertexShader();
	assert(vertexShader != nullptr);

	_skinMeshProgram = std::make_unique<GlShaderProgram>(*vertexShader, *_glFragmentShader);
}

void ShaderPrograms::makeInstancedMeshProgram() {
	if (_instancedMeshProgram != nullptr)
		return;

	assert(_resources.expired() == false);

	auto &vertexShader = _resources.lock()->getInstancedMeshVertexShader();
	assert(vertexShader != nullptr);

	_instancedMeshProgram = std::make_unique<GlShaderProgram>(*vertexShader, *_glFragmentShader);
}

void ShaderPrograms::makeProgram(Scene::MeshType meshType) {
	switch (meshType) {
	case Scene::MeshType::Standard: makeMeshProgram(); break;
	case Scene::MeshType::Skin: makeSkinMeshProgram(); break;
	case Scene::MeshType::Instanced: makeInstancedMeshProgram(); break;
	}
}

const std::unique_ptr<GlShaderProgram> &ShaderPrograms::getMeshProgram() const {
	return _meshProgram;
}

const std::unique_ptr<GlShaderProgram> &ShaderPrograms::getSkinMeshProgram() const {
	return _skinMeshProgram;
}

const std::unique_ptr<GlShaderProgram> &ShaderPrograms::getInstancedMeshProgram() const {
	return _instancedMeshProgram;
}

GlShaderProgram *ShaderPrograms::getProgram(Scene::MeshType meshType) const {
	switch (meshType) {
	case Scene::MeshType::Standard: return _meshProgram.get();
	case Scene::MeshType::Skin: return _skinMeshProgram.get();
	case Scene::MeshType::Instanced: return _instancedMeshProgram.get();
	}
	return nullptr;
}

} // namespace Stone::Render::OpenGL
