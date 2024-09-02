// Copyright 2024 Stone-Engine

#include "Core/Image/ImageSource.hpp"

#include "Core/Assets/Bundle.hpp"
#include "Core/Image/ImageData.hpp"

namespace Stone::Core::Image {

ImageSource::ImageSource(const std::shared_ptr<Assets::Bundle> &bundle, const std::string &filepath, Channel channels)
	: Assets::Resource(bundle, filepath), _channels(channels) {
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
		_loadedImage = std::make_shared<ImageData>(getFullPath(), _channels);
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
		loadData(false);
	}
	return _loadedImage;
}

} // namespace Stone::Core::Image
