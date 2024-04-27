// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene {

class LightNode : public PivotNode {
public:
	STONE_ABSTRACT_NODE(LightNode);

	explicit LightNode(const std::string &name = "light");
	LightNode(const LightNode &other);

	virtual ~LightNode();

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] virtual bool isCastingShadow() const;

protected:
	float _intensity;
	glm::vec3 _color;

	[[nodiscard]] const char *_termClassColor() const override;
};

class AmbientLightNode : public LightNode {
public:
	STONE_NODE(AmbientLightNode);

	explicit AmbientLightNode(const std::string &name = "ambientlight");
	AmbientLightNode(const AmbientLightNode &other);

	~AmbientLightNode() override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;
};

class PointLightNode : public LightNode {
public:
	STONE_NODE(PointLightNode);

	explicit PointLightNode(const std::string &name = "pointlight");
	PointLightNode(const PointLightNode &other);

	~PointLightNode() override;

    [[nodiscard]] const glm::vec3 &getAttenuation() const;
    void setAttenuation(const glm::vec3 &attenuation);

    [[nodiscard]] const glm::vec3 &getSpecular() const;
    void setSpecular(const glm::vec3 &specular);

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

protected:
	glm::vec3 _attenuation;
	glm::vec3 _specular;
};

class CastingLightNode : public LightNode {
public:
	STONE_ABSTRACT_NODE(CastingLightNode);

	explicit CastingLightNode(const std::string &name = "castinglight");
	CastingLightNode(const CastingLightNode &other);

	~CastingLightNode() override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] bool isCastingShadow() const override;
	void setCastingShadow(bool castShadow);

	[[nodiscard]] const glm::mat4 &getProjectionMatrix() const;

	[[nodiscard]] float getShadowClipNear() const;
	void setShadowClipNear(float shadowClipNear);

	[[nodiscard]] float getShadowClipFar() const;
	void setShadowClipFar(float shadowClipFar);

	[[nodiscard]] const glm::ivec2 &getShadowMapSize() const;
	void setShadowMapSize(const glm::ivec2 &shadowMapSize);

protected:
	bool _castShadow;
	float _shadowClipNear;
	float _shadowClipFar;
	glm::ivec2 _shadowMapSize;
	glm::mat4 _projectionMatrix;

	virtual void _updateProjectionMatrix() = 0;

	[[nodiscard]] const char *_termClassColor() const override;
};

class DirectionalLightNode : public CastingLightNode {
public:
	STONE_NODE(DirectionalLightNode);

	explicit DirectionalLightNode(const std::string &name = "directionallight");
	DirectionalLightNode(const DirectionalLightNode &other);

	~DirectionalLightNode() override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] bool isInfinite() const;
	void setInfinite(bool infinite);

	[[nodiscard]] const glm::vec2 &getShadowOrthoSize() const;
	void setShadowOrthoSize(const glm::vec2 &shadowOrthoSize);

protected:
	bool _infinite;
	glm::vec2 _shadowOrthoSize;

	void _updateProjectionMatrix() override;
};

class SpotLightNode : public CastingLightNode {
public:
	STONE_NODE(SpotLightNode);

	explicit SpotLightNode(const std::string &name = "spotlight");
	SpotLightNode(const SpotLightNode &other);

	~SpotLightNode() override;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	[[nodiscard]] float getConeAngle() const;
	void setConeAngle(float coneAngle);

	[[nodiscard]] float getConeAttenuation() const;
	void setConeAttenuation(float coneAttenuation);

protected:
	float _coneAngle;
	float _coneAttenuation;

	void _updateProjectionMatrix() override;
};

} // namespace Stone::Scene
