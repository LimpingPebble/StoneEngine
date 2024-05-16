// Copyright 2024 Stone-Engine

#include "Core/Image/ImageSource.hpp"

#include "Core/Image/ImageData.hpp"

namespace Stone::Image {

ImageSource::ImageSource(std::string path, Channel channels) : _filepath(std::move(path)), _channels(channels) {
}

const char *ImageSource::getClassName() const {
	return "ImageSource";
}

std::ostream &ImageSource::writeToStream(std::ostream &stream, bool closing_bracer) const {
	stream << "{path:" << _filepath << ",channels:" << _channels;
	if (closing_bracer)
		stream << "}";
	return stream;
}

const std::string &ImageSource::getFilePath() const {
	return _filepath;
}

Channel ImageSource::getChannels() const {
	return _channels;
}

Size ImageSource::getSize() const {
	return _size;
}

void ImageSource::loadData(bool force) {
	if (force || _loadedImage == nullptr) {
		_loadedImage = std::make_shared<ImageData>(_filepath, _channels);
		_loadedImage->_source = std::dynamic_pointer_cast<ImageSource>(shared_from_this());
		_channels = _loadedImage->getChannels();
		_size = _loadedImage->getSize();
	}
}

void ImageSource::unloadData() {
	_loadedImage = nullptr;
}

bool ImageSource::isLoaded() const {
	return _loadedImage != nullptr;
}

std::shared_ptr<ImageData> ImageSource::getLoadedImage() const {
	return _loadedImage;
}

std::shared_ptr<ImageData> ImageSource::getLoadedImage(bool loadIfNeeded) {
	if (loadIfNeeded && !isLoaded()) {
		loadData();
	}
	return _loadedImage;
}

} // namespace Stone::Image