// Copyright 2024 Stone-Engine

#pragma once

#include <glm/vec2.hpp>
#include <iostream>
#include <memory>

namespace Stone {

class Image : public std::enable_shared_from_this<Image> {
public:
	Image() = default;
	Image(const Image &other) = delete;

	virtual ~Image();

	enum class Channel : int {
		Grey = 1,
		GreyAlpha = 2,
		Rgb = 3,
		RgbAlpha = 4
	};

	static std::shared_ptr<Image> load(const std::string &filepath, Channel channel = Channel::RgbAlpha);

	std::ostream &writeToStream(std::ostream &stream) const;

	void setSize(const glm::uvec2 &size);
	[[nodiscard]] const glm::uvec2 &getSize() const;

	void setChannels(uint32_t channels);
	[[nodiscard]] uint32_t getChannels() const;

	void setData(const uint8_t *data);
	[[nodiscard]] const uint8_t *getData() const;

protected:
	glm::uvec2 _size = glm::uvec2(0);
	uint32_t _channels = 0;
	const uint8_t *_data = nullptr;
};

std::ostream &operator<<(std::ostream &stream, const Image &image);

} // namespace Stone
