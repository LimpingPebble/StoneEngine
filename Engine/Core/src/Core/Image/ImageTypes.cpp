// Copyright 2024 Stone-Engine

#include "Core/Image/ImageTypes.hpp"

namespace Stone::Image {

std::ostream &operator<<(std::ostream &stream, Channel channel) {
	switch (channel) {
	case Channel::GREY: stream << "GREY"; break;
	case Channel::DUAL: stream << "DUAL"; break;
	case Channel::RGB: stream << "RGB"; break;
	case Channel::RGBA: stream << "RGBA"; break;
	}
	return stream;
}

} // namespace Stone::Image
