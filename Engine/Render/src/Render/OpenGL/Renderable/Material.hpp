// Copyright 2024 Stone-Engine

#pragma once

#include "../RendererDefaults.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Material.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Stone::Render::OpenGL {

class Material : public Scene::IRendererObject {
public:
	Material(Scene::Material &material, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _material(material), _renderer(renderer) {
	}

	~Material() override {
		if (_gl_meshProgram != 0)
			glDeleteProgram(_gl_meshProgram);
		if (_gl_skinMeshProgram != 0)
			glDeleteProgram(_gl_skinMeshProgram);
		if (_gl_instancedMeshProgram != 0)
			glDeleteProgram(_gl_instancedMeshProgram);
	}

	void render(Scene::RenderContext &context) override {
		auto getLoc = [this](const char *name) -> GLint {
			GLint loc = glGetUniformLocation(_lastUsedProgram, name);
			if (loc == -1) {
				throw std::runtime_error("Failed to get location of uniform: " + std::string(name));
			}
			return loc;
		};

		glUniformMatrix4fv(getLoc("u_mat_projection"), 1, GL_FALSE, glm::value_ptr(context.mvp.projMatrix));
		glUniformMatrix4fv(getLoc("u_mat_view"), 1, GL_FALSE, glm::value_ptr(context.mvp.viewMatrix));
		glUniformMatrix4fv(getLoc("u_mat_model"), 1, GL_FALSE, glm::value_ptr(context.mvp.modelMatrix));
	}

	void makeMeshProgram() {
		if (_gl_meshProgram != 0)
			return;

		if (_renderer.expired())
			return;

		auto renderer = _renderer.lock();

		auto vertexShader = renderer->getOpenGLRendererDefaults().getMeshVertexShader();

		assert(vertexShader != nullptr);

		// std::shared_ptr<Scene::FragmentShader> fragmentShader =
		// 	_material.getFragmentShader() ? _material.getFragmentShader() :
		// 	renderer->getOpenGLRendererDefaults().getFragmentShader();

		// assert(fragmentShader != nullptr);

		auto glFragmentShader = renderer->getOpenGLRendererDefaults().getFragmentShader();
		//  fragmentShader->getRendererObject<FragmentShader>();

		_gl_meshProgram = glCreateProgram();
		glAttachShader(_gl_meshProgram, vertexShader->getGLShader());
		glAttachShader(_gl_meshProgram, glFragmentShader->getGLShader());
		glLinkProgram(_gl_meshProgram);

		GLint success;
		glGetProgramiv(_gl_meshProgram, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infoLog[1024];
			glGetProgramInfoLog(_gl_meshProgram, 1024, nullptr, infoLog);
			throw std::runtime_error("Failed to link program: " + std::string(infoLog));
		}
	}

	void makeSkinMeshProgram() {
		if (_gl_skinMeshProgram != 0)
			return;
	}

	void makeInstancedMeshProgram() {
		if (_gl_instancedMeshProgram != 0)
			return;
	}

	void useMeshProgram() {
		glUseProgram(_gl_meshProgram);
		_lastUsedProgram = _gl_meshProgram;
	}

	void useSkinMeshProgram() {
		glUseProgram(_gl_skinMeshProgram);
		_lastUsedProgram = _gl_skinMeshProgram;
	}

	void useInstancedMeshProgram() {
		glUseProgram(_gl_instancedMeshProgram);
		_lastUsedProgram = _gl_instancedMeshProgram;
	}

private:
	Scene::Material &_material;
	std::weak_ptr<OpenGLRenderer> _renderer;

	GLuint _gl_meshProgram = 0;
	GLuint _gl_skinMeshProgram = 0;
	GLuint _gl_instancedMeshProgram = 0;

	GLuint _lastUsedProgram = 0;
};

} // namespace Stone::Render::OpenGL
