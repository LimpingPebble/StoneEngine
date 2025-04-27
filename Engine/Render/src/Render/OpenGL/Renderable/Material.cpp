// Copyright 2024 Stone-Engine

#include "Material.hpp"

#include "../OpenGLResources.hpp"
#include "FragmentShader.hpp"

namespace Stone::Render::OpenGL {

Material::Material(Scene::Material &material, const std::shared_ptr<OpenGLRenderer> &renderer)
	: IOpenGLRendererObject(renderer), _material(material) {
	assert(renderer);
	assert(renderer->getResources());
	if (_material.getFragmentShader() == nullptr) {
		Scene::MaterialInputSignature inputs(std::dynamic_pointer_cast<Scene::Material>(material.shared_from_this()));
		_shaderPrograms = renderer->getResources()->getStandardShaderPrograms(inputs);
	} else {
		_shaderPrograms = _material.getFragmentShader()->getRendererObject<FragmentShader>()->getShaderPrograms();
	}
	assert(_shaderPrograms);

#ifndef NDEBUG
	int textureCount = 0;
	material.forEachTextures([&textureCount](auto, auto) { ++textureCount; });
	if (textureCount >= 32) {
		// TODO: Use log module
		std::cerr << "Material " << material.getId() << " has more than 32 textures" << std::endl;
	}
#endif
}

void Material::setUniforms(Scene::MeshType meshType) {
	GlShaderProgram *program = _shaderPrograms->getProgram(meshType);
	assert(program != nullptr);
	program->use();

	_material.forEachVectors([program](const auto &loc, glm::vec3 vec) { program->setUniform(loc, vec); });
	_material.forEachScalars([program](const auto &loc, float scalar) { program->setUniform(loc, scalar); });

	int textureIndex = 0;
	_material.forEachTextures(
		[program, &textureIndex](const auto &loc, const std::shared_ptr<Scene::Texture> &texture) {
			if (textureIndex >= 32)
				return;

			assert(texture->isDirty() == false);
			auto rendererTexture = texture->getRendererObject<Texture>();
			program->setUniformTexture(loc, *rendererTexture, textureIndex);
			++textureIndex;
		});
}

const std::shared_ptr<ShaderPrograms> &Material::getShaderPrograms() const {
	return _shaderPrograms;
}


} // namespace Stone::Render::OpenGL
