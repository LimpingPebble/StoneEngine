#include "config.h"
#include "Render/OpenGL/Shader/ShaderGenerator.hpp"
#include "Scene/Renderable/Shader.hpp"
#include "Utils/FileSystem.hpp"
#include "Utils/Json.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>

static time_t getLastmodifiedTimeOfFile(const char *filename) {
	struct stat result;
	if (stat(filename, &result) == 0) {
		auto mod_time = result.st_mtime;
		return mod_time;
	} else
		throw std::runtime_error("File does not exist");
}

Stone::Scene::MaterialInputSignature parseMaterialInputSignature(const Json::Value &json) {
	Stone::Scene::MaterialInputSignature params;

	auto &params_obj = json.get<Json::Object>();

	for (auto [key, value] : params_obj) {
		Stone::Scene::MaterialInputSignature::Type type;

		if (!value.is<std::string>())
			throw std::runtime_error("Invalid type for key " + key);

		const std::string &type_str = value.get<std::string>();
		if (type_str == "scalar")
			type = Stone::Scene::MaterialInputSignature::Type::Scalar;
		else if (type_str == "vector2")
			type = Stone::Scene::MaterialInputSignature::Type::Vector2;
		else if (type_str == "vector3")
			type = Stone::Scene::MaterialInputSignature::Type::Vector3;
		else if (type_str == "vector4")
			type = Stone::Scene::MaterialInputSignature::Type::Vector4;
		else if (type_str == "texture")
			type = Stone::Scene::MaterialInputSignature::Type::Texture;
		else
			throw std::runtime_error("Invalid type " + type_str);

		params.setParamWithName(key, type);
	}

	return params;
}

std::string to_string(Stone::Scene::MaterialInputSignature::Type type) {
	switch (type) {
	case Stone::Scene::MaterialInputSignature::Type::None: return "none";
	case Stone::Scene::MaterialInputSignature::Type::Scalar: return "scalar";
	case Stone::Scene::MaterialInputSignature::Type::Vector2: return "vector2";
	case Stone::Scene::MaterialInputSignature::Type::Vector3: return "vector3";
	case Stone::Scene::MaterialInputSignature::Type::Vector4: return "vector4";
	case Stone::Scene::MaterialInputSignature::Type::Texture: return "texture";
	}
	return "";
}

void generateShaderOutput(const char *input_file, const char *output_file) {

	Json::Value input_json;
	Json::parseFile(input_file, input_json);
	Json::Object &input_json_obj(input_json.get<Json::Object>());

	std::ofstream output_stream(output_file, std::ios::out | std::ios::trunc);

	std::shared_ptr<Stone::Scene::FragmentShader> shader = nullptr;
	if (input_json_obj.find("shader") != input_json_obj.end()) {
		shader = std::make_shared<Stone::Scene::FragmentShader>();
		shader->setContent(Stone::Scene::AShader::ContentType::SourceCode,
						   input_json_obj["shader"].get<Json::String>());
		input_json_obj.erase("shader");
	}

	Stone::Scene::MaterialInputSignature params = parseMaterialInputSignature(input_json);

	Stone::Render::OpenGL::ShaderGenerator generator;
	std::cout << "Generating shader: {" << std::endl;
#define __PRINT_SHADER_PARAM(param) std::cout << "    " << #param << " " << to_string(params.param) << std::endl;
	FOR_EACH_SHADER_PARAMETERS(__PRINT_SHADER_PARAM)
	std::cout << "}" << std::endl;

	generator.generateOpenGlForwardFragmentShader(params, shader, output_stream);
}

std::string input;
std::string output;

void generateShader() {
	try {
		generateShaderOutput(input.c_str(), output.c_str());
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown error occurred" << std::endl;
	}
}

void printUsage() {
	std::cout << "Usage: glsl_generator <input> <output> [-f]" << std::endl;
	std::cout << "-f : Watch for file change" << std::endl;
}

int main(int argc, const char *argv[]) {
	if (argc < 3) {
		printUsage();
		return 1;
	}

	input = argv[1];
	output = argv[2];

	generateShader();

	if (argc >= 4 && std::string(argv[3]).find('f') != std::string::npos) {
		while (true) {
			time_t last_modified = getLastmodifiedTimeOfFile(input.c_str());
			while (last_modified == getLastmodifiedTimeOfFile(input.c_str())) {
				// sleep for a short period
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
			}

			generateShader();
		}
		return 0;
	}
}
