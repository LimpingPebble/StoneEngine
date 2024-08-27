// Copyright 2024 Stone-Engine

#include "Core/Image/ImageAlbum.hpp"
#include "Core/Image/ImageSource.hpp"

namespace Stone::Image {

const char *Album::getClassName() const {
    return "Album";
}

std::ostream &Album::writeToStream(std::ostream &stream, bool closing_bracer) const {
	stream << "{";
    for (const auto &image : _images) {
        stream << image.first << ":";
        image.second->writeToStream(stream, true);
    }
    if (closing_bracer)
        stream << "}";
    return stream;
}

std::shared_ptr<ImageSource> Album::loadImage(const std::string &path, Channel channels, bool reload) {
    auto it = _images.find(path);
    if (it != _images.end()) {
        if (reload) {
            it->second->unloadData();
            it->second->loadData();
        }
        return it->second;
    } else {
        auto image = std::make_shared<ImageSource>(path, channels);
        _images[path] = image;
        image->loadData();
        return image;
    }
}

void Album::removeImage(const std::string &path) {
    auto it = _images.find(path);
    if (it != _images.end()) {
        it->second->unloadData();
        _images.erase(it);
    }
}

void Album::clearImages() {
    for (auto &image : _images) {
        image.second->unloadData();
    }
    _images.clear();
}

std::vector<std::string> Album::getImagePaths() const {
    std::vector<std::string> paths;
    paths.reserve(_images.size());
    for (const auto &image : _images) {
        paths.push_back(image.first);
    }
    return paths;
}

std::vector<std::shared_ptr<ImageSource>> Album::getImages() const {
    std::vector<std::shared_ptr<ImageSource>> images;
    images.reserve(_images.size());
    for (const auto &image : _images) {
        images.push_back(image.second);
    }
    return images;
}

std::shared_ptr<ImageSource> Album::getImage(const std::string &path) const {
    auto it = _images.find(path);
    if (it != _images.end()) {
        return it->second;
    }
    return nullptr;
}

}; // namespace Stone::Image
