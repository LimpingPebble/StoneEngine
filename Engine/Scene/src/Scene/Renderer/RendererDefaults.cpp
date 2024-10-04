// Copyright 2024 Stone-Engine

#include "Scene/Renderer/RendererDefaults.hpp"

#include "Scene/Renderable/Material.hpp"

namespace Stone::Scene {

RendererDefaults::RendererDefaults() : _defaultMaterial(nullptr) {
}

const std::shared_ptr<Material> &RendererDefaults::getDefaultMaterial() const {
	return _defaultMaterial;
}

} // namespace Stone::Scene
