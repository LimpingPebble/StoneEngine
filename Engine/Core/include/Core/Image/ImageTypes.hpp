// Copyright 2024 Stone-Engine

#pragma once

#include <glm/vec2.hpp>
#include <iostream>

namespace Stone::Image {

using Size = glm::ivec2;

enum class Channel : int {
	GREY = 1,
	DUAL = 2,
	RGB = 3,
	RGBA = 4
};

std::ostream &operator<<(std::ostream &stream, Channel channel);

} // namespace Stone::Image
