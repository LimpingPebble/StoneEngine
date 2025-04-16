// Copyright 2024 Stone-Engine

#include "MeshNode.hpp"

#include "../OpenGLResources.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Mesh.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

MeshNode::MeshNode(Scene::MeshNode &meshNode, const std::shared_ptr<OpenGLRenderer> &renderer)
	: _meshNode(meshNode), _renderer(renderer), _material(nullptr), _shaderPrograms(nullptr) {

	auto usedMaterial = _meshNode.getUsedMaterial();
	if (usedMaterial) {
		assert(usedMaterial->isDirty() == false);
		_material = usedMaterial->getRendererObject<Material>();
	}

	if (_material) {
		_shaderPrograms = _material->getShaderPrograms();
	} else {
		_shaderPrograms = renderer->getResources()->getStandardShaderPrograms(Scene::MaterialInputSignature(nullptr));
	}
	assert(_shaderPrograms);

	_shaderPrograms->makeMeshProgram();
}

void MeshNode::render(Scene::RenderContext &context) {
	auto mesh = _meshNode.getMesh();
	if (mesh == nullptr)
		return;

	auto rendererMesh = mesh->getRendererObject<RendererMesh>();
	if (rendererMesh == nullptr) {
		return;
	}
	const VRAMMesh &vramMesh = rendererMesh->getVRAMMesh();

	GlShaderProgram *program = _shaderPrograms->getMeshProgram().get();
	program->use();

	if (_material) {
		_material->setUniforms(Scene::MeshType::Standard);
	}

	program->setUniform("u_lights_count", 0);
	program->setUniform("u_mat_model", context.mvp.modelMatrix);
	program->setUniform("u_mat_view", context.mvp.viewMatrix);
	program->setUniform("u_mat_projection", context.mvp.projMatrix);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBindVertexArray(vramMesh.elementsBuffer);
	glDrawElements(GL_TRIANGLES, vramMesh.numIndices, GL_UNSIGNED_INT, NULL);
}

} // namespace Stone::Render::OpenGL
