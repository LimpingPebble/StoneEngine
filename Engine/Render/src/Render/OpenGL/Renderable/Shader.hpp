// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Shader.hpp"
#include "Utils/FileSystem.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

GLuint compileSource(const std::string &source, GLenum type);

GLuint loadSpirv(const char *spirv_content, GLsizei spirv_length, GLenum type);

class ShaderBase : public Scene::IRendererObject {
public:
	ShaderBase(Scene::AShader &shader, const std::shared_ptr<OpenGLRenderer> &renderer, GLenum type)
		: _shader(shader), _renderer(renderer) {

		auto [contentType, content] = shader.getContent();

		using ContentType = Scene::AShader::ContentType;
		switch (contentType) {
		case ContentType::SourceCode: _gl_shader = compileSource(content, type); break;
		case ContentType::SourceFile: _gl_shader = compileSource(Utils::readTextFile(content), type); break;
		case ContentType::CompiledCode: _gl_shader = loadSpirv(content.data(), content.size(), type); break;
		case ContentType::CompiledFile:
			auto fileContent = Utils::readBinaryFile(content);
			_gl_shader = loadSpirv(fileContent.data(), fileContent.size(), type);
			break;
		}
	}

	~ShaderBase() override {
		if (_gl_shader != 0)
			glDeleteShader(_gl_shader);
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

	GLuint getGLShader() const {
		return _gl_shader;
	}

protected:
	Scene::AShader &_shader;
	std::weak_ptr<OpenGLRenderer> _renderer;

	GLuint _gl_shader = 0;
};

class SourceVertexShader : public Scene::AShader {
	STONE_OBJECT(SourceVertexShader);

public:
	SourceVertexShader(const std::string &source) : Scene::AShader(ContentType::SourceCode, source) {
	}

	SourceVertexShader(const SourceVertexShader &other) = delete;

	~SourceVertexShader() override = default;
};

class VertexShader : public ShaderBase {
public:
	VertexShader(Scene::AShader &shader, const std::shared_ptr<OpenGLRenderer> &renderer)
		: ShaderBase(shader, renderer, GL_VERTEX_SHADER) {
	}

	static std::shared_ptr<VertexShader> makeStandardMeshShader(const std::shared_ptr<OpenGLRenderer> &renderer);
	static std::shared_ptr<VertexShader> makeStandardSkinMeshShader(const std::shared_ptr<OpenGLRenderer> &renderer);
};

class FragmentShader : public ShaderBase {
public:
	FragmentShader(Scene::AShader &shader, const std::shared_ptr<OpenGLRenderer> &renderer)
		: ShaderBase(shader, renderer, GL_FRAGMENT_SHADER) {
	}

	static std::shared_ptr<FragmentShader> makeStandardMeshShader(const std::shared_ptr<OpenGLRenderer> &renderer);

protected:
};


} // namespace Stone::Render::OpenGL
