// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Shader/ShaderInputSignature.hpp"

namespace Stone::Scene {

class ShaderGenerator {


public:
	ShaderGenerator() = default;

	~ShaderGenerator() = default;

	void generateFragmentShaderTemplate(const ShaderInputSignature &params, std::ostream &output);

	void generateOpenGlForwardFragmentShader(const ShaderInputSignature &params, class FragmentShader *shader,
											 std::ostream &output);
};

} // namespace Stone::Scene
