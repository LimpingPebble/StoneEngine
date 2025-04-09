// Copyright 2024 Stone-Engine

#include "Scene/Shader/ShaderGenerator.hpp"

namespace Stone::Scene {

std::string to_glsl(ShaderParameters::Type type) {
	switch (type) {
	case ShaderParameters::Type::Scalar: return "float"; break;
	case ShaderParameters::Type::Vector2: return "vec2"; break;
	case ShaderParameters::Type::Vector3: return "vec3"; break;
	case ShaderParameters::Type::Vector4: return "vec4"; break;
	case ShaderParameters::Type::Texture: return "sampler2D"; break;
	default: return ""; break;
	}
}

void ShaderGenerator::generateFragmentShaderTemplate(const ShaderParameters &params, std::ostream &output) {

	output << "// Stone shader template" << std::endl;

	auto add_uniform_param = [&output](const char *name, ShaderParameters::Type type) {
		if (type != ShaderParameters::Type::None)
			output << "// " << name << ": " << to_glsl(type) << std::endl;
	};

#define __ADD_UNIFORM_PARAM(PARAM) add_uniform_param(#PARAM, params.PARAM);

	FOR_EACH_SHADER_PARAMETERS(__ADD_UNIFORM_PARAM);
	output << std::endl;

	output << "void customShader() {" << std::endl;
	output << " // diffuse = vec3(1, 0, 0);" << std::endl;
	output << "	// TODO: Implement shader generation logic here" << std::endl;
	output << "}" << std::endl;
}

} // namespace Stone::Scene
