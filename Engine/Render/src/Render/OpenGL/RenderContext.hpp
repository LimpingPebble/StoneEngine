// Copyright 2024 StoneEngine

#pragma once

#include "Scene/RenderContext.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class OpenGLRenderer;

struct RenderContext : public Scene::RenderContext {
	OpenGLRenderer *renderer;
};

} // namespace Stone::Render::OpenGL
