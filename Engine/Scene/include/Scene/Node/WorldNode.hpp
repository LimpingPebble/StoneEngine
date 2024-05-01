// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/Node.hpp"

namespace Stone::Scene {

class CameraNode;

class WorldNode : public Node {
public:
	STONE_NODE(WorldNode);

	explicit WorldNode(const std::string &name = "world");
	WorldNode(const WorldNode &other) = default;

	~WorldNode() override = default;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	void setRenderer(const std::shared_ptr<ISceneRenderer> &renderer);
	[[nodiscard]] std::shared_ptr<ISceneRenderer> getRenderer() const;

	void setActiveCamera(const std::shared_ptr<CameraNode> &camera);
	[[nodiscard]] std::shared_ptr<CameraNode> getActiveCamera() const;

	void initializeRenderContext(RenderContext &context) const;

protected:
	std::shared_ptr<ISceneRenderer> _renderer;
	std::weak_ptr<CameraNode> _activeCamera;

	[[nodiscard]] const char *_termClassColor() const override;
};

} // namespace Stone::Scene
