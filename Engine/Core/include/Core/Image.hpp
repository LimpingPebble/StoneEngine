// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"

#include <glm/vec2.hpp>

namespace Stone::Core {

class Image : public Object {
public:
	Image() = default;
	Image(const Image &other) = default;

	~Image() override = default;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

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

} // namespace Stone::Core
