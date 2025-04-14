// Copyright 2024 Stone-Engine

#include "OpenGLDirector.hpp"

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Render/OpenGL/RenderContext.hpp"
#include "Scene/Node/WorldNode.hpp"


namespace Stone::Render::OpenGL {

static void initializeOpenGL() {
	static bool initialized = false;
	if (initialized)
		return;

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW");
	}
	initialized = true;
}

OpenGLDirector::OpenGLDirector(const std::shared_ptr<OpenGLRenderer> &renderer, RenderingMethod method)
	: _method(method), _frameSize(0, 0), _renderer(renderer), _gBuffer() {
}

void OpenGLDirector::initialize(const std::pair<uint32_t, uint32_t> &frameSize) {
	initializeOpenGL();
	updateFrameSize(frameSize);
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

void OpenGLDirector::renderWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	switch (_method) {
	case RenderingMethod::Forward: renderWorldForward(world); break;
	case RenderingMethod::Deferred: renderWorldDeffered(world); break;
	}
}

void OpenGLDirector::updateFrameSize(const std::pair<uint32_t, uint32_t> &frameSize) {
	_frameSize = frameSize;

	if (_gBuffer)
		_gBuffer.reset();

	if (_method == RenderingMethod::Deferred) {
		_gBuffer = std::make_unique<GlGBuffer>(_frameSize.first, _frameSize.second);
	}
}

void OpenGLDirector::renderWorldForward(const std::shared_ptr<Scene::WorldNode> &world) {
	OpenGL::RenderContext context;
	context.renderer = _renderer.lock();
	world->initializeRenderContext(context);

	_clearViewport();

	world->render(context);
}

void OpenGLDirector::renderWorldDeffered(const std::shared_ptr<Scene::WorldNode> &world) {
	OpenGL::RenderContext context;
	context.renderer = _renderer.lock();
	context.gBuffer = _gBuffer.get();
	world->initializeRenderContext(context);

	_gBuffer->bind();
	_clearViewport();

	world->render(context);

	_gBuffer->unbind();

	_gBuffer->render();
}

void OpenGLDirector::_clearViewport() {
	glViewport(0, 0, static_cast<GLsizei>(_frameSize.first), static_cast<GLsizei>(_frameSize.second));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


} // namespace Stone::Render::OpenGL
