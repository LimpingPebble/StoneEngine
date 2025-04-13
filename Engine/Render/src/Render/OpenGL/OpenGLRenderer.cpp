// Copyright 2024 Stone-Engine


#include "Render/OpenGL/OpenGLRenderer.hpp"

#include "GlElements/GlGBuffer.hpp"
#include "OpenGLResources.hpp"
#include "RenderContext.hpp"
#include "RendererObjectFactory.hpp"
#include "Scene/Node/WorldNode.hpp"

#include <GL/glew.h>


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

OpenGLRenderer::OpenGLRenderer(RendererSettings &settings)
	: Renderer(), _frameSize(settings.frame_size), _method(settings.rendering_method), _resources(nullptr) {
}

OpenGLRenderer::~OpenGLRenderer() {
	std::cout << "OpenGLRenderer destroyed" << std::endl;
}

void OpenGLRenderer::updateRenderablesInNode(const std::shared_ptr<Scene::Node> &rootNode) {
	OpenGL::RendererObjectFactory factory(std::static_pointer_cast<OpenGLRenderer>(shared_from_this()));
	factory.updateRenderablesInNode(rootNode);
}

void OpenGLRenderer::renderWorld(const std::shared_ptr<Scene::WorldNode> &world) {

	switch (_method) {
	case RenderingMethod::Deferred:
		{
			// Reset framebuffers

			_gBuffer->bind();
			glViewport(0, 0, static_cast<GLsizei>(_frameSize.first), static_cast<GLsizei>(_frameSize.second));
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			OpenGL::RenderContext context;
			context.renderer = std::static_pointer_cast<OpenGLRenderer>(shared_from_this());
			context.gBuffer = _gBuffer.get();

			world->initializeRenderContext(context);
			world->render(context);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			_gBuffer->render();
			break;
		}
	case RenderingMethod::Forward:
		{
			OpenGL::RenderContext context;
			context.renderer = std::static_pointer_cast<OpenGLRenderer>(shared_from_this());

			glViewport(0, 0, static_cast<GLsizei>(_frameSize.first), static_cast<GLsizei>(_frameSize.second));
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			world->initializeRenderContext(context);
			world->render(context);

			break;
		}
	}
}

void OpenGLRenderer::updateFrameSize(std::pair<uint32_t, uint32_t> size) {
	_frameSize = size;
	if (_method == RenderingMethod::Deferred) {
		if (_gBuffer)
			_gBuffer.reset();
		_gBuffer = std::make_unique<GlGBuffer>(_frameSize.first, _frameSize.second);
	}
}

void OpenGLRenderer::initialize() {
	initializeOpenGL();
	updateFrameSize(_frameSize);
	std::cout << "OpenGLRenderer created" << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	_resources = std::make_shared<OpenGLResources>(std::static_pointer_cast<OpenGLRenderer>(shared_from_this()));
}

RenderingMethod OpenGLRenderer::getRenderingMethod() const {
	return _method;
}

const std::shared_ptr<OpenGLResources> &OpenGLRenderer::getOpenGLResources() const {
	return _resources;
}


} // namespace Stone::Render::OpenGL
