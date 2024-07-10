// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "ImageTypes.hpp"

namespace Stone::Image {

class ImageData;

/**
 * @brief ImageSource class. This class does not contain any image data,
 * it is used to hold reference to the image file and the channels of the image.
 * It is used to load the image data when needed and hold the reference to the loaded image data.
 */
class ImageSource : public Core::Object {
public:
	/**
	 * @brief Construct a new Image Header object
	 *
	 * @param path The path to the image file
	 * @param channels The number of channels in the image
	 */
	explicit ImageSource(std::string path, Channel channels = Channel::RGBA);

	ImageSource() = delete;
	ImageSource(const ImageSource &other) = delete;

	~ImageSource() override = default;

	const char *getClassName() const override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] const std::string &getFilePath() const;
	[[nodiscard]] Channel getChannels() const;
	[[nodiscard]] Size getSize() const;

	void loadData(bool force = false);
	void unloadData();

	[[nodiscard]] bool isLoaded() const;
	[[nodiscard]] std::shared_ptr<ImageData> getLoadedImage() const;
	[[nodiscard]] std::shared_ptr<ImageData> getLoadedImage(bool loadIfNeeded = false);

protected:
	std::string _filepath;
	Channel _channels;
	Size _size = Size(0);

	std::shared_ptr<ImageData> _loadedImage = nullptr;
};

} // namespace Stone::Image
