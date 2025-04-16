// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderable/SkinMesh.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

struct VRAMSkinMesh {

	VRAMSkinMesh(const std::shared_ptr<Scene::DynamicSkinMesh> &skinMesh) {
	}

	~VRAMSkinMesh() {
	}
};

} // namespace Stone::Render::OpenGL
