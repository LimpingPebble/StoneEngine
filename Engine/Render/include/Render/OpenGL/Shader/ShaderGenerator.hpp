// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderable/Material.hpp"

namespace Stone::Scene {
class FragmentShader;
}

namespace Stone::Render::OpenGL {

class ShaderGenerator {


public:
	ShaderGenerator() = default;

	~ShaderGenerator() = default;

	void generateFragmentShaderTemplate(const Scene::MaterialInputSignature &params, std::ostream &output);

	void generateOpenGlForwardFragmentShader(const Scene::MaterialInputSignature &params,
											 const std::shared_ptr<Scene::FragmentShader> &shader,
											 std::ostream &output);
};

} // namespace Stone::Render::OpenGL
