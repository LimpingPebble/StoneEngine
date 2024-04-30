// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/ISceneRenderer.hpp"

#include <memory>

namespace Stone::Render {

class Renderer : public std::enable_shared_from_this<Renderer>, public Scene::ISceneRenderer {
public:
	Renderer() = default;
	Renderer(const Renderer &) = delete;

	virtual ~Renderer() = default;
};

} // namespace Stone::Render
