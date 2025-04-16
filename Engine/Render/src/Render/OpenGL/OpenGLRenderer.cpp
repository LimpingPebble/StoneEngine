// Copyright 2024 Stone-Engine


#include "Render/OpenGL/OpenGLRenderer.hpp"

#include "OpenGLDirector.hpp"
#include "OpenGLResources.hpp"
#include "RendererObjectFactory.hpp"


namespace Stone::Render::OpenGL {

void OpenGLRenderer::initialize(RendererSettings &settings) {
	const auto this_shared = std::static_pointer_cast<OpenGLRenderer>(shared_from_this());
	_resources = std::make_shared<OpenGLResources>(this_shared);
	_director = std::make_shared<OpenGLDirector>(this_shared, settings.rendering_method);
	_director->initialize(settings.frame_size);
}

void OpenGLRenderer::updateRenderablesInNode(const std::shared_ptr<Scene::Node> &rootNode) {
	OpenGL::RendererObjectFactory factory(std::static_pointer_cast<OpenGLRenderer>(shared_from_this()));
	factory.updateRenderablesInNode(rootNode);
}

void OpenGLRenderer::renderWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	_director->renderWorld(world);
}

void OpenGLRenderer::updateFrameSize(const std::pair<uint32_t, uint32_t> &size) {
	_director->updateFrameSize(size);
}

const std::shared_ptr<OpenGLDirector> &OpenGLRenderer::getDirector() const {
	return _director;
}

const std::shared_ptr<OpenGLResources> &OpenGLRenderer::getResources() const {
	return _resources;
}


} // namespace Stone::Render::OpenGL
