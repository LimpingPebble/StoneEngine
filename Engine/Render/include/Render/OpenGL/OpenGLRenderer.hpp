// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/RendererSettings.hpp"
#include "Render/Renderer.hpp"

namespace Stone::Scene {
class WorldNode;
}

namespace Stone::Render::OpenGL {

class OpenGLRenderer : public Renderer {
public:
	OpenGLRenderer() = delete;
	explicit OpenGLRenderer(RendererSettings &settings);
	OpenGLRenderer(const OpenGLRenderer &) = delete;

	~OpenGLRenderer() override;

	/** Renderer */

	void updateDataForWorld(const std::shared_ptr<Scene::WorldNode> &world) override;
	const std::unique_ptr<Scene::RendererDefaults> &getRendererDefaults() const override;
	void renderWorld(const std::shared_ptr<Scene::WorldNode> &world) override;

	void updateFrameSize(std::pair<uint32_t, uint32_t> size) override;

private:
	std::pair<uint32_t, uint32_t> _frameSize;

	std::unique_ptr<class RendererInternals> _internals;
	friend class RendererInternals;

	std::unique_ptr<Scene::RendererDefaults> _defaults;
};

} // namespace Stone::Render::OpenGL
