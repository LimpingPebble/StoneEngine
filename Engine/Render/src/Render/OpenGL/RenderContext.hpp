// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderer/RenderContext.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

struct GlGBuffer;

struct RenderContext : public Scene::RenderContext {
	GlGBuffer *gBuffer;
};

} // namespace Stone::Render::OpenGL
