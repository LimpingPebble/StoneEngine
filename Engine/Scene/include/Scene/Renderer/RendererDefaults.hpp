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

	virtual ~RendererDefaults() = default;

	/**
	 * @brief Get the default material used when a mesh has no material set.
	 */
	const std::shared_ptr<Material> &getMaterial() const;

protected:
	std::shared_ptr<Material> _material;
};

} // namespace Stone::Scene
