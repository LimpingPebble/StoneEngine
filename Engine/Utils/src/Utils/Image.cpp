// Copyright 2024 Stone-Engine

#include "Utils/Image.hpp"

#include "Utils/Glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>
#include <Utils/stb_image.h>

namespace Stone {

Image::~Image() {
	if (_data) {
		stbi_image_free(const_cast<uint8_t *>(_data));
	}
}

std::shared_ptr<Image> Image::load(const std::string &filepath, Channel channels) {
	int width, height, bpp;
	uint8_t *data = stbi_load(filepath.c_str(), &width, &height, &bpp, static_cast<int>(channels));
	if (!data) {
		throw std::runtime_error("Failed to load image: " + filepath);
	}

	auto image = std::make_shared<Image>();
	image->setSize(glm::uvec2(width, height));
	image->setChannels(bpp);
	image->setData(data);

	return image;
}

std::ostream &Image::writeToStream(std::ostream &stream) const {
	return stream << "{size:" << _size << ",channels:" << _channels << "}";
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

std::ostream &operator<<(std::ostream &stream, const Image &image) {
	return image.writeToStream(stream);
}

} // namespace Stone
