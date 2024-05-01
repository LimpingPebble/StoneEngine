// Copyright 2024 Stone-Engine

#pragma once

#include "SceneTypes.hpp"

#include <memory>

namespace Stone::Scene {

class ISceneRenderer {
public:
	virtual void updateDataForWorld(const std::shared_ptr<WorldNode> &world) = 0;
	virtual void renderWorld(const std::shared_ptr<WorldNode> &world) = 0;
};

} // namespace Stone::Scene
