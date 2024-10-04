// Copyright 2024 Stone-Engine

#pragma once

#include "SceneTypes.hpp"

namespace Stone::Scene {

/**
 * @brief Default values for a renderer.
 */
class RendererDefaults {
public:
	RendererDefaults();
	RendererDefaults(const RendererDefaults &) = delete;

	~RendererDefaults() = default;

	/**
	 * @brief Get the default material used when a mesh has no material set.
	 */
	const std::shared_ptr<Material> &getDefaultMaterial() const;

protected:
	std::shared_ptr<Material> _defaultMaterial;
};

} // namespace Stone::Scene
