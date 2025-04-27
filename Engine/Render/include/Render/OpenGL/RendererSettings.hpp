// Copyright 2024 Stone-Engine

#pragma once

#include <functional>
#include <memory>


namespace Stone::Render::OpenGL {

enum class RenderingMethod {
	Forward,
	Deferred
};

struct RendererSettings {
	std::pair<uint32_t, uint32_t> frame_size = {};
	RenderingMethod rendering_method = RenderingMethod::Deferred;
};

} // namespace Stone::Render::OpenGL
