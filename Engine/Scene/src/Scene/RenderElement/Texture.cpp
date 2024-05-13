// Copyright 2024 Stone-Engine

#include "Scene/RenderElement/Texture.hpp"

#include "Scene/RendererObjectManager.hpp"
#include "Utils/Image.hpp"

namespace Stone::Scene {

const char *Texture::getClassName() const {
	return "Texture";
}

std::ostream &Texture::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",image:" << _image;
	if (closing_bracer)
		stream << "}";
	return stream;
}

void Texture::updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) {
	manager->updateTexture(std::static_pointer_cast<Texture>(shared_from_this()));
}

void Texture::setImage(std::shared_ptr<Image> image) {
	_image = std::move(image);
	markDirty();
}

std::shared_ptr<Image> Texture::getImage() const {
	return _image;
}

} // namespace Stone::Scene
