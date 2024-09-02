// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Texture.hpp"

#include "Core/Image/ImageSource.hpp"
#include "Scene/RendererObjectManager.hpp"

namespace Stone::Scene {

std::ostream &Texture::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",image:" << *_image;
	if (closing_bracer)
		stream << "}";
	return stream;
}

void Texture::updateRenderObject(RendererObjectManager &manager) {
	manager.updateTexture(std::static_pointer_cast<Texture>(shared_from_this()));
}

void Texture::setImage(const std::shared_ptr<Core::Image::ImageSource> &image) {
	_image = image;
	markDirty();
}

const std::shared_ptr<Core::Image::ImageSource> &Texture::getImage() const {
	return _image;
}

void Texture::setWrap(TextureWrap wrap) {
	_wrap = wrap;
	markDirty();
}

TextureWrap Texture::getWrap() const {
	return _wrap;
}

void Texture::setMinFilter(TextureFilter filter) {
	_minFilter = filter;
	markDirty();
}

TextureFilter Texture::getMinFilter() const {
	return _minFilter;
}

void Texture::setMagFilter(TextureFilter filter) {
	_magFilter = filter;
	markDirty();
}

TextureFilter Texture::getMagFilter() const {
	return _magFilter;
}

} // namespace Stone::Scene
