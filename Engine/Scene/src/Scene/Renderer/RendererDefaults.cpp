// Copyright 2024 Stone-Engine

#include "Scene/Renderer/RendererDefaults.hpp"

#include "Scene/Renderable/Material.hpp"

namespace Stone::Scene {

RendererDefaults::RendererDefaults() : _material(nullptr) {
}

const std::shared_ptr<Material> &RendererDefaults::getMaterial() const {
	return _material;
}

} // namespace Stone::Scene
