// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/RendererSettings.hpp"
#include "Render/Renderer.hpp"


namespace Stone::Render::OpenGL {

class OpenGLResources;
struct GlGBuffer;

class OpenGLRenderer : public Renderer {
public:
	OpenGLRenderer() = delete;
	explicit OpenGLRenderer(RendererSettings &settings);
	OpenGLRenderer(const OpenGLRenderer &) = delete;

	~OpenGLRenderer() override;

	/** Renderer */

	void updateRenderablesInNode(const std::shared_ptr<Scene::Node> &rootNode) override;
	void renderWorld(const std::shared_ptr<Scene::WorldNode> &world) override;

	void updateFrameSize(std::pair<uint32_t, uint32_t> size) override;

	void initialize();
	RenderingMethod getRenderingMethod() const;
	const std::shared_ptr<OpenGLResources> &getOpenGLResources() const;

private:
	std::pair<uint32_t, uint32_t> _frameSize;
	const RenderingMethod _method;
	std::shared_ptr<OpenGLResources> _resources;
	std::unique_ptr<GlGBuffer> _gBuffer;
};

} // namespace Stone::Render::OpenGL
