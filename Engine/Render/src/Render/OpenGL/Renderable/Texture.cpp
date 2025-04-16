// Copyright 2024 Stone-Engine

#include "Texture.hpp"

#include "Core/Image/ImageData.hpp"
#include "Core/Image/ImageSource.hpp"

namespace Stone::Render::OpenGL {

GLuint convert(Core::Image::Channel channel) {
	switch (channel) {
	case Core::Image::Channel::GREY: return GL_RED;
	case Core::Image::Channel::DUAL: return GL_RG;
	case Core::Image::Channel::RGB: return GL_RGB;
	case Core::Image::Channel::RGBA: return GL_RGBA;
	default: return GL_RGBA;
	}
}

GLuint convert(Scene::TextureFilter filter, bool mipmap) {
	switch (filter) {
	case Scene::TextureFilter::Nearest: return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
	case Scene::TextureFilter::Linear: return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	case Scene::TextureFilter::Cubic: return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	default: return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	}
}

GLuint convert(Scene::TextureWrap wrap) {
	switch (wrap) {
	case Scene::TextureWrap::Repeat: return GL_REPEAT;
	case Scene::TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
	case Scene::TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
	case Scene::TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
	default: return GL_REPEAT;
	}
}

Texture::Texture(Scene::Texture &texture, const std::shared_ptr<OpenGLRenderer> &renderer)
	: IOpenGLRendererObject(renderer), _gl_texture(0) {
	std::shared_ptr<Core::Image::ImageSource> imageSource = texture.getImage();
	if (imageSource == nullptr) {
		return;
	}

	imageSource->loadData();
	std::shared_ptr<Core::Image::ImageData> imageData = imageSource->getLoadedImage();

	GLuint format = convert(imageData->getChannels());
	glGenTextures(1, &_gl_texture);
	if (_gl_texture == 0) {
		std::runtime_error("Failed to create texture buffer.");
	}
	glBindTexture(GL_TEXTURE_2D, _gl_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, imageData->getSize().x, imageData->getSize().y, 0, format, GL_UNSIGNED_BYTE,
				 imageData->getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convert(texture.getWrap()));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, convert(texture.getWrap()));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convert(texture.getMinFilter(), true));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convert(texture.getMagFilter(), false));
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(GLsizei width, GLsizei height, GLint internalFormat, GLenum format, GLenum type, GLint wrap,
				 GLint filter)
	: IOpenGLRendererObject(nullptr), _gl_texture(0) {
	glGenTextures(1, &_gl_texture);
	if (_gl_texture == 0) {
		std::runtime_error("Failed to create texture buffer.");
	}
	glBindTexture(GL_TEXTURE_2D, _gl_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

Texture::~Texture() {
	if (_gl_texture != 0) {
		glDeleteTextures(1, &_gl_texture);
	}
}


} // namespace Stone::Render::OpenGL
