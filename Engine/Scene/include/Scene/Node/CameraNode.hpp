// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene {

class CameraNode : public PivotNode {
public:
	STONE_ABSTRACT_NODE(CameraNode);

	explicit CameraNode(const std::string &name = "camera");
	CameraNode(const CameraNode &other);

	virtual ~CameraNode();

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] virtual const glm::mat4 getProjectionMatrix() const = 0;

	[[nodiscard]] float getNear() const;
	void setNear(float near);

	[[nodiscard]] float getFar() const;
	void setFar(float far);

protected:
	float _near;
	float _far;

	[[nodiscard]] const char *_termClassColor() const override;
};

class PerspectiveCameraNode : public CameraNode {
public:
	STONE_NODE(PerspectiveCameraNode);

	explicit PerspectiveCameraNode(const std::string &name = "perspective_camera");
	PerspectiveCameraNode(const PerspectiveCameraNode &other);

	~PerspectiveCameraNode() override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] const glm::mat4 getProjectionMatrix() const override;

	[[nodiscard]] float getFov() const;
	void setFov(float fov);

	[[nodiscard]] float getAspect() const;
	void setAspect(float aspect);

protected:
	float _fov;
	float _aspect;
};

class OrthographicCameraNode : public CameraNode {
public:
	STONE_NODE(OrthographicCameraNode);

	explicit OrthographicCameraNode(const std::string &name = "orthographic_camera");
	OrthographicCameraNode(const OrthographicCameraNode &other);

	~OrthographicCameraNode() override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] const glm::mat4 getProjectionMatrix() const override;

	[[nodiscard]] glm::vec2 getSize() const;
	void setSize(const glm::vec2 &size);

protected:
	glm::vec2 _size;
};

} // namespace Stone::Scene
