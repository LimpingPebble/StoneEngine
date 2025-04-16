// Copyright 2024 Stone-Engine

#pragma once

#include "IOpenGLRendererObject.hpp"
#include "Scene/Renderable/Texture.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {


class Texture : public IOpenGLRendererObject {
public:
	Texture(Scene::Texture &texture, const std::shared_ptr<OpenGLRenderer> &renderer);

	Texture(GLsizei width, GLsizei height, GLint internalFormat, GLenum format, GLenum type, GLint wrap, GLint filter);

	~Texture() override;

	void render(Scene::RenderContext &) override {
	}

	GLuint getGlTexture() const {
		return _gl_texture;
	}

private:
	GLuint _gl_texture = 0;
};

} // namespace Stone::Render::OpenGL
