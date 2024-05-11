// Copyright 2024 Stone-Engine

#include "Core/Image.hpp"

#include "Utils/Glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>

namespace Stone::Core {

std::ostream &Image::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",size:" << _size;
	stream << ",channels:" << _channels;
	if (closing_bracer)
		stream << "}";
	return stream;
}

void Image::setSize(const glm::uvec2 &size) {
	_size = size;
}

const glm::uvec2 &Image::getSize() const {
	return _size;
}

void Image::setChannels(uint32_t channels) {
	_channels = channels;
}

uint32_t Image::getChannels() const {
	return _channels;
}

void Image::setData(const uint8_t *data) {
	_data = data;
}

const uint8_t *Image::getData() const {
	return _data;
}

} // namespace Stone::Core
