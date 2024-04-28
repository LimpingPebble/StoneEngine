// Copyright 2024 Stone-Engine

#include "Scene/Node/LightNode.hpp"

#include "Utils/Glm.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Stone::Scene {

STONE_ABSTRACT_NODE_IMPLEMENTATION(LightNode);

LightNode::LightNode(const std::string &name) : PivotNode(name), _intensity(1.0f), _color(1.0f) {
}

LightNode::LightNode(const LightNode &other) : PivotNode(other), _intensity(other._intensity), _color(other._color) {
}

LightNode::~LightNode() {
}

std::ostream &LightNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	PivotNode::writeToStream(stream, false);
	stream << ",intensity:" << _intensity;
	stream << ",color:" << _color;
	if (closing_bracer)
		stream << "}";
	return stream;
}

bool LightNode::isCastingShadow() const {
	return false;
}

const char *LightNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_YELLOW;
}

STONE_NODE_IMPLEMENTATION(AmbientLightNode);

AmbientLightNode::AmbientLightNode(const std::string &name) : LightNode(name) {
}

AmbientLightNode::AmbientLightNode(const AmbientLightNode &other) : LightNode(other) {
}

AmbientLightNode::~AmbientLightNode() {
}

std::ostream &AmbientLightNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	return LightNode::writeToStream(stream, closing_bracer);
}

STONE_NODE_IMPLEMENTATION(PointLightNode);

PointLightNode::PointLightNode(const std::string &name) : LightNode(name) {
}

PointLightNode::PointLightNode(const PointLightNode &other) : LightNode(other) {
}

PointLightNode::~PointLightNode() {
}

const glm::vec3 &PointLightNode::getAttenuation() const {
	return _attenuation;
}

void PointLightNode::setAttenuation(const glm::vec3 &attenuation) {
	_attenuation = attenuation;
}

const glm::vec3 &PointLightNode::getSpecular() const {
	return _specular;
}

void PointLightNode::setSpecular(const glm::vec3 &specular) {
	_specular = specular;
}

std::ostream &PointLightNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	LightNode::writeToStream(stream, false);
	stream << ",attenuation:" << _attenuation;
	stream << ",specular:" << _specular;
	if (closing_bracer)
		stream << "}";
	return stream;
}

STONE_ABSTRACT_NODE_IMPLEMENTATION(CastingLightNode);

CastingLightNode::CastingLightNode(const std::string &name)
	: LightNode(name), _castShadow(true), _shadowClipNear(0.1f), _shadowClipFar(100.0f), _shadowMapSize(1024) {
}

CastingLightNode::CastingLightNode(const CastingLightNode &other)
	: LightNode(other), _castShadow(other._castShadow), _shadowClipNear(other._shadowClipNear),
	  _shadowClipFar(other._shadowClipFar), _shadowMapSize(other._shadowMapSize) {
}

CastingLightNode::~CastingLightNode() {
}

std::ostream &CastingLightNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	LightNode::writeToStream(stream, false);
	stream << ",castShadow:" << _castShadow;
	stream << ",shadowClipNear:" << _shadowClipNear;
	stream << ",shadowClipFar:" << _shadowClipFar;
	stream << ",shadowMapSize:" << _shadowMapSize;
	if (closing_bracer)
		stream << "}";
	return stream;
}

bool CastingLightNode::isCastingShadow() const {
	return _castShadow;
}

void CastingLightNode::setCastingShadow(bool castShadow) {
	_castShadow = castShadow;
}

const glm::mat4 &CastingLightNode::getProjectionMatrix() const {
	return _projectionMatrix;
}

float CastingLightNode::getShadowClipNear() const {
	return _shadowClipNear;
}

void CastingLightNode::setShadowClipNear(float shadowClipNear) {
	_shadowClipNear = shadowClipNear;
	_updateProjectionMatrix();
}

float CastingLightNode::getShadowClipFar() const {
	return _shadowClipFar;
}

void CastingLightNode::setShadowClipFar(float shadowClipFar) {
	_shadowClipFar = shadowClipFar;
	_updateProjectionMatrix();
}

const glm::ivec2 &CastingLightNode::getShadowMapSize() const {
	return _shadowMapSize;
}

void CastingLightNode::setShadowMapSize(const glm::ivec2 &shadowMapSize) {
	_shadowMapSize = shadowMapSize;
}

const char *CastingLightNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_ORANGE;
}

STONE_NODE_IMPLEMENTATION(DirectionalLightNode);

DirectionalLightNode::DirectionalLightNode(const std::string &name)
	: CastingLightNode(name), _infinite(true), _shadowOrthoSize(10.0f, 10.0f) {
	_updateProjectionMatrix();
}

DirectionalLightNode::DirectionalLightNode(const DirectionalLightNode &other)
	: CastingLightNode(other), _infinite(other._infinite), _shadowOrthoSize(other._shadowOrthoSize) {
	_updateProjectionMatrix();
}

DirectionalLightNode::~DirectionalLightNode() {
}

std::ostream &DirectionalLightNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	CastingLightNode::writeToStream(stream, false);
	stream << ",infinite:" << _infinite;
	stream << ",shadowOrthoSize:" << _shadowOrthoSize;
	if (closing_bracer)
		stream << "}";
	return stream;
}

bool DirectionalLightNode::isInfinite() const {
	return _infinite;
}

void DirectionalLightNode::setInfinite(bool infinite) {
	_infinite = infinite;
}

const glm::vec2 &DirectionalLightNode::getShadowOrthoSize() const {
	return _shadowOrthoSize;
}

void DirectionalLightNode::setShadowOrthoSize(const glm::vec2 &shadowOrthoSize) {
	_shadowOrthoSize = shadowOrthoSize;
	_updateProjectionMatrix();
}

void DirectionalLightNode::_updateProjectionMatrix() {
	_projectionMatrix = glm::ortho(-_shadowOrthoSize.x / 2.0f, _shadowOrthoSize.x / 2.0f, -_shadowOrthoSize.y / 2.0f,
								   _shadowOrthoSize.y / 2.0f, _shadowClipNear, _shadowClipFar);
}

STONE_NODE_IMPLEMENTATION(SpotLightNode);

SpotLightNode::SpotLightNode(const std::string &name)
	: CastingLightNode(name), _coneAngle(glm::radians(45.0f)), _coneAttenuation(0.0f) {
	_updateProjectionMatrix();
}

SpotLightNode::SpotLightNode(const SpotLightNode &other)
	: CastingLightNode(other), _coneAngle(other._coneAngle), _coneAttenuation(other._coneAttenuation) {
	_updateProjectionMatrix();
}

SpotLightNode::~SpotLightNode() {
}

std::ostream &SpotLightNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	CastingLightNode::writeToStream(stream, false);
	stream << ",coneAngle:" << _coneAngle;
	stream << ",coneAttenuation:" << _coneAttenuation;
	if (closing_bracer)
		stream << "}";
	return stream;
}

float SpotLightNode::getConeAngle() const {
	return _coneAngle;
}

void SpotLightNode::setConeAngle(float coneAngle) {
	_coneAngle = coneAngle;
	_updateProjectionMatrix();
}

float SpotLightNode::getConeAttenuation() const {
	return _coneAttenuation;
}

void SpotLightNode::setConeAttenuation(float coneAttenuation) {
	_coneAttenuation = coneAttenuation;
}

void SpotLightNode::_updateProjectionMatrix() {
	_projectionMatrix = glm::perspective(_coneAngle, 1.0f, _shadowClipNear, _shadowClipFar);
}

} // namespace Stone::Scene
