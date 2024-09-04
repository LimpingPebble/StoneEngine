// Copyright 2024 Stone-Engine

#include "Scene/Renderer/RendererDefaults.hpp"
#include "Scene/Renderable/Material.hpp"

namespace Stone::Scene {

RendererDefaults::RendererDefaults() {
    _defaultMaterial = std::make_shared<Material>();
}

const std::shared_ptr<Material>& RendererDefaults::getDefaultMaterial() const {
    return _defaultMaterial;
}

void RendererDefaults::setDefaultMaterial(const std::shared_ptr<Material> &material) {
    _defaultMaterial = material;
}

} // namespace Stone::Scene
