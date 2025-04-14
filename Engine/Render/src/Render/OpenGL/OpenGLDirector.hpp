// Copyright 2024 Stone-Engine

#pragma once

#include "GlElements/GlGBuffer.hpp"
#include "Render/OpenGL/RendererSettings.hpp"

namespace Stone::Scene {
class Node;
class WorldNode;
} // namespace Stone::Scene

namespace Stone::Render::OpenGL {

class OpenGLRenderer;
struct GlGBuffer;

class OpenGLDirector {

public:
	OpenGLDirector(const std::shared_ptr<OpenGLRenderer> &renderer, RenderingMethod method);

	virtual ~OpenGLDirector() = default;

	void initialize(const std::pair<uint32_t, uint32_t> &frameSize);

	void updateFrameSize(const std::pair<uint32_t, uint32_t> &frameSize);

	void renderWorld(const std::shared_ptr<Scene::WorldNode> &world);
	void renderWorldForward(const std::shared_ptr<Scene::WorldNode> &world);
	void renderWorldDeffered(const std::shared_ptr<Scene::WorldNode> &world);

	[[nodiscard]]
	RenderingMethod getRenderingMethod() const {
		return _method;
	}

private:
	void _clearViewport();

	const RenderingMethod _method;
	std::pair<uint32_t, uint32_t> _frameSize;
	std::weak_ptr<OpenGLRenderer> _renderer;
	std::unique_ptr<GlGBuffer> _gBuffer;
};

} // namespace Stone::Render::OpenGL
