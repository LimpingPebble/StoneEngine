// Copyright 2024 Stone-Engine

#include "Scene/Shader/ShaderInputSignature.hpp"

#include "Scene/Renderable/Material.hpp"

namespace Stone::Scene {

#define __INITIALIZE_PARAM(param) param(Type::None),
ShaderInputSignature::ShaderInputSignature() : FOR_EACH_SHADER_PARAMETERS(__INITIALIZE_PARAM) _() {
}

void ShaderInputSignature::setParamWithName(const std::string &name, Type value) {
	using ParamSetter = std::function<void(ShaderInputSignature &, Type)>;

#define __MAP_NAME_TO_PARAM(param)                                                                                     \
	{                                                                                                                  \
		#param, [](ShaderInputSignature &matParams, Type value) {                                                          \
			matParams.param = value;                                                                                   \
		}                                                                                                              \
	}                                                                                                                  \
	,

	const static std::unordered_map<std::string, ParamSetter> paramSetters = {
		FOR_EACH_SHADER_PARAMETERS(__MAP_NAME_TO_PARAM) //
	};

	auto it = paramSetters.find(name);
	if (it != paramSetters.end()) {
		it->second(*this, value);
	}
}

void ShaderInputSignature::setFromMaterial(const Material &material) {
	material.forEachScalars([this](const Material::Location &location, float value) {
		(void)value;
		if (std::holds_alternative<std::string>(location)) {
			const std::string &name(std::get<std::string>(location));
			setParamWithName(name, Type::Scalar);
		}
	});
	material.forEachVectors([this](const Material::Location &location, glm::vec3 value) {
		(void)value;
		if (std::holds_alternative<std::string>(location)) {
			const std::string &name(std::get<std::string>(location));
			setParamWithName(name, Type::Vector3);
		}
	});
	material.forEachTextures([this](const Material::Location &location, auto value) {
		(void)value;
		if (std::holds_alternative<std::string>(location)) {
			const std::string &name(std::get<std::string>(location));
			setParamWithName(name, Type::Texture);
		}
	});
}

} // namespace Stone::Scene
