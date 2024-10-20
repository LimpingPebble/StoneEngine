// Copyright 2024 Stone-Engine

#include "Shader.hpp"

namespace Stone::Render::OpenGL {

GLuint compileSource(const std::string &source, GLenum type) {
	GLuint shaderId = glCreateShader(type);
	if (shaderId == 0) {
		throw std::runtime_error("Failed to create shader.");
	}

	const char *source_c = source.c_str();
	glShaderSource(shaderId, 1, &source_c, nullptr);
	glCompileShader(shaderId);

	GLint compiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint log_length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log(log_length);
		glGetShaderInfoLog(shaderId, log_length, &log_length, log.data());

		glDeleteShader(shaderId);

		throw std::runtime_error("Failed to compile shader: " + std::string(log.data()));
	}

	return shaderId;
}

GLuint loadSpirv(const char *spirv_content, GLsizei spirv_length, GLenum type) {
	(void)type;

	GLuint shaderId = glCreateShader(GL_VERTEX_SHADER);
	if (shaderId == 0) {
		throw std::runtime_error("Failed to create shader.");
	}

	glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, spirv_content, spirv_length);
	glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);

	int compiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint log_length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log(log_length);
		glGetShaderInfoLog(shaderId, log_length, &log_length, log.data());

		glDeleteShader(shaderId);

		throw std::runtime_error("Failed to compile shader: " + std::string(log.data()));
	}

	return shaderId;
}

const char *basicVertexShaderSource = R"shader(
#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 uv;

uniform mat3 u_mat_normal;
uniform mat4 u_mat_projection;
uniform mat4 u_mat_view;
uniform mat4 u_mat_model;
uniform vec3 u_camera_position;

out VS_OUT {
    vec4 position;
    vec3 wposition;
    vec2 uv;
    vec3 wnormal;
    vec3 wtangent;
    vec3 wbitangent;
} vs_out;

void main()
{
    vs_out.wposition = vec3(u_mat_model * vec4(position, 1.0));
    gl_Position = u_mat_projection * u_mat_view * vec4(vs_out.wposition, 1.0);
    vs_out.position = gl_Position;
    vs_out.wnormal = u_mat_normal * normal;
    vs_out.wtangent = u_mat_normal * tangent;
    vs_out.wbitangent = u_mat_normal * bitangent;
    vs_out.uv = uv;
};

)shader";

const char *basicSkinVertexShaderSource = R"shader(
#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in ivec4 boneIDs;
layout(location = 4) in vec4 boneWeights;

out vec3 fragNormal;
out vec2 fragUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

void main() {
    fragNormal = normal;
    fragUV = uv;

    mat4 boneTransform = mat4(0.0);
    for (int i = 0; i < 4; i++) {
        boneTransform += boneWeights[i] * bones[boneIDs[i]];
    }

    gl_Position = projection * view * model * boneTransform * vec4(position, 1.0);
}

)shader";

const char *basicFragmentShaderSource = R"shader(
#version 400 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

)shader";

std::shared_ptr<VertexShader> VertexShader::makeStandardMeshShader(const std::shared_ptr<OpenGLRenderer> &renderer) {
	auto sourceShader = std::make_shared<SourceVertexShader>(basicVertexShaderSource);
	auto glShader = std::make_shared<VertexShader>(*sourceShader, renderer);
	return glShader;
}

std::shared_ptr<VertexShader>
VertexShader::makeStandardSkinMeshShader(const std::shared_ptr<OpenGLRenderer> &renderer) {
	auto sourceShader = std::make_shared<SourceVertexShader>(basicSkinVertexShaderSource);
	auto glShader = std::make_shared<VertexShader>(*sourceShader, renderer);
	return glShader;
}

std::shared_ptr<FragmentShader>
FragmentShader::makeStandardMeshShader(const std::shared_ptr<OpenGLRenderer> &renderer) {
	auto sceneShader =
		std::make_shared<Scene::FragmentShader>(Scene::AShader::ContentType::SourceCode, basicFragmentShaderSource);
	auto glShader = std::make_shared<FragmentShader>(*sceneShader, renderer);
	return glShader;
}


} // namespace Stone::Render::OpenGL
