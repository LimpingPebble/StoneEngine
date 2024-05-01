// Copyright 2024 Stone-Engine

#include "Scene/RenderElement/Material.hpp"

#include "Scene/RenderElement/Texture.hpp"
#include "Scene/RendererObjectManager.hpp"
#include "Utils/Glm.hpp"

namespace Stone::Scene {

const char *Material::getClassName() const {
	return "Material";
}

std::ostream &Material::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",textures:{";
	for (auto &it : _textures)
		stream << it.first << ":" << (it.second ? std::to_string(it.second->getId()) : "null") << ",";
	stream << "},vectors:{";
	for (auto &it : _vectors)
		stream << it.first << ":" << it.second << ",";
	stream << "},scalars:{";
	for (auto &it : _scalars)
		stream << it.first << ":" << it.second << ",";
	stream << "}";
	if (closing_bracer)
		stream << "}";
	return stream;
}

void Material::updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) {
	manager->updateMaterial(std::static_pointer_cast<Material>(shared_from_this()));
}

void Material::setTextureParameter(const std::string &name, std::shared_ptr<Texture> texture) {
	_textures[name] = std::move(texture);
	markDirty();
}

std::shared_ptr<Texture> Material::getTextureParameter(const std::string &name) const {
	auto it = _textures.find(name);
	if (it != _textures.end()) {
		return it->second;
	}
	return nullptr;
}

void Material::setVectorParameter(const std::string &name, const glm::vec3 &vector) {
	_vectors[name] = vector;
	markDirty();
}

glm::vec3 Material::getVectorParameter(const std::string &name) const {
	auto it = _vectors.find(name);
	if (it != _vectors.end()) {
		return it->second;
	}
	return glm::vec3(0.0f);
}

void Material::setScalarParameter(const std::string &name, float scalar) {
	_scalars[name] = scalar;
	markDirty();
}

float Material::getScalarParameter(const std::string &name) const {
	auto it = _scalars.find(name);
	if (it != _scalars.end()) {
		return it->second;
	}
	return 0.0f;
}

} // namespace Stone::Scene
