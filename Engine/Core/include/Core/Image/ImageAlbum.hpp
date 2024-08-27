// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "ImageTypes.hpp"

#include <map>
#include <vector>

namespace Stone::Image {

class ImageSource;

/**
 * @brief Album class. This class is used to hold a collection of images referenced by their path.
 */
class Album : public Core::Object {
public:
	Album() = default;
	Album(const Album &other) = delete;

	~Album() override = default;

	const char *getClassName() const override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	std::shared_ptr<ImageSource> loadImage(const std::string &path, Channel channels = Channel::RGBA, bool reload = false);
	void removeImage(const std::string &path);
	void clearImages();

	[[nodiscard]] std::vector<std::string> getImagePaths() const;

	[[nodiscard]] std::vector<std::shared_ptr<ImageSource>> getImages() const;

	[[nodiscard]] std::shared_ptr<ImageSource> getImage(const std::string &path) const;

protected:
	std::map<std::string, std::shared_ptr<ImageSource>> _images;
};

} // namespace Stone::Image
