// Copyright 2024 Stone-Engine

#include "Scene/RenderElement/Texture.hpp"

#include "Core/Image.hpp"
#include "Scene/RendererObjectManager.hpp"

namespace Stone::Scene {

const char *Texture::getClassName() const {
	return "Texture";
}

std::ostream &Texture::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",image:" << (_image ? std::to_string(_image->getId()) : "null");
	if (closing_bracer)
		stream << "}";
	return stream;
}

void Texture::updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) {
	manager->updateTexture(std::static_pointer_cast<Texture>(shared_from_this()));
}

void Texture::setImage(std::shared_ptr<Core::Image> image) {
	_image = std::move(image);
	markDirty();
}

std::shared_ptr<Core::Image> Texture::getImage() const {
	return _image;
}

} // namespace Stone::Scene
