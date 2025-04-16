// Copyright 2024 Stone-Engine

#include "OpenGLResources.hpp"

#include "GlElements/ShaderPrograms.hpp"
#include "OpenGLDirector.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Material.hpp"

namespace Stone::Render::OpenGL {

OpenGLResources::OpenGLResources(const std::shared_ptr<OpenGLRenderer> &renderer)
	: std::enable_shared_from_this<OpenGLResources>(), _renderer(renderer) {
}

const std::weak_ptr<OpenGLRenderer> OpenGLResources::getRenderer() const {
	return _renderer;
}

// MARK: Vertex Shaders

const std::unique_ptr<GlVertexShader> &OpenGLResources::getMeshVertexShader() {
	if (_meshVertexShader == nullptr) {
		_meshVertexShader = GlVertexShader::makeStandardMeshShader();
	}
	return _meshVertexShader;
}

const std::unique_ptr<GlVertexShader> &OpenGLResources::getSkinMeshVertexShader() {
	if (_skinMeshVertexShader == nullptr) {
		_skinMeshVertexShader = GlVertexShader::makeStandardSkinMeshShader();
	}
	return _skinMeshVertexShader;
}

const std::unique_ptr<GlVertexShader> &OpenGLResources::getInstancedMeshVertexShader() {
	if (_instancedMeshVertexShader == nullptr) {
		_instancedMeshVertexShader = GlVertexShader::makeStandardInstancedMeshShader();
	}
	return _instancedMeshVertexShader;
}

const std::unique_ptr<GlVertexShader> &OpenGLResources::getVertexShader(Scene::MeshType meshType) {
	switch (meshType) {
	case Scene::MeshType::Standard: return getMeshVertexShader();
	case Scene::MeshType::Skin: return getSkinMeshVertexShader();
	case Scene::MeshType::Instanced: return getInstancedMeshVertexShader();
	}
	assert(false);
}

const std::unique_ptr<GlFragmentShader> &OpenGLResources::getFragmentShader(Scene::MaterialInputSignature params) {
	auto renderer = getRenderer().lock();
	assert(renderer != nullptr);
	auto it = _fragmentShaders.find(params);
	if (it == _fragmentShaders.end()) {
		switch (renderer->getDirector()->getRenderingMethod()) {
		case RenderingMethod::Forward:
			return (_fragmentShaders[params] = GlFragmentShader::makeStandardForwardShader(params));
		case RenderingMethod::Deferred:
			return (_fragmentShaders[params] = GlFragmentShader::makeStandardDeferredShader(params));
		}
	} else {
		return it->second;
	}
}

const std::shared_ptr<ShaderPrograms> &
OpenGLResources::getStandardShaderPrograms(Scene::MaterialInputSignature params) {
	auto it = _standardsShaderPrograms.find(params);
	if (it == _standardsShaderPrograms.end()) {
		return (_standardsShaderPrograms[params] = std::make_shared<ShaderPrograms>(params, shared_from_this()));
	} else {
		return it->second;
	}
}

} // namespace Stone::Render::OpenGL
