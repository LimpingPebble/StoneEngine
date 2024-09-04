// Copyright 2024 StoneEngine

#pragma once

#include "Scene/RenderContext.hpp"

#include "Renderable/Material.hpp"

namespace Stone::Render::OpenGL {

class RendererInternals {
public:
    RendererInternals() = default;
    RendererInternals(const RendererInternals &other) = delete;

    ~RendererInternals() = default;

    static RendererInternals& getFrom(OpenGLRenderer &renderer) {
        return *(renderer._internals);
    }

};

} // namespace Stone::Render::OpenGL
