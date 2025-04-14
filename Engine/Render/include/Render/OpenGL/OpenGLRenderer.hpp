// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/RendererSettings.hpp"
#include "Render/Renderer.hpp"


namespace Stone::Render::OpenGL {

class OpenGLDirector;
class OpenGLResources;

class OpenGLRenderer : public Renderer {
public:
	OpenGLRenderer() = default;

	~OpenGLRenderer() override = default;

	void initialize(RendererSettings &settings);

	/** Renderer */

	void updateRenderablesInNode(const std::shared_ptr<Scene::Node> &rootNode) override;
	void renderWorld(const std::shared_ptr<Scene::WorldNode> &world) override;

	void updateFrameSize(std::pair<uint32_t, uint32_t> size) override;

	const std::shared_ptr<OpenGLDirector> &getDirector() const;
	const std::shared_ptr<OpenGLResources> &getResources() const;

private:
	std::shared_ptr<OpenGLDirector> _director;
	std::shared_ptr<OpenGLResources> _resources;
};

} // namespace Stone::Render::OpenGL
