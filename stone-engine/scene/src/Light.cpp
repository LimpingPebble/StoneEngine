// Copyright 2024 Stone-Engine

#include "Light.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace STN
{

    namespace Scene
    {

        Light::Light(const std::string &name)
            : Node(name), _intensity(1.0f), _color(1.0f)
        {
        }

        Light::Light(const Light &other)
            : Node(other), _intensity(other._intensity), _color(other._color)
        {
        }

        Light::~Light()
        {
        }

        const char *Light::getClassName() const
        {
            return "Light";
        }

        std::string Light::debugDescription() const
        {
            std::string str = Node::debugDescription();
            str.pop_back();
            str += ",intensity:" + std::to_string(_intensity);
            str += ",color:[" + std::to_string(_color.r) + "," + std::to_string(_color.g) + "," + std::to_string(_color.b) + "]}";
            return str;
        }

        bool Light::isCastingShadow() const
        {
            return false;
        }

        const char *Light::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_YELLOW;
        }

        AmbientLight::AmbientLight(const std::string &name)
            : Light(name)
        {
        }

        AmbientLight::AmbientLight(const AmbientLight &other)
            : Light(other)
        {
        }

        AmbientLight::~AmbientLight()
        {
        }

        const char *AmbientLight::getClassName() const
        {
            return "AmbientLight";
        }

        std::string AmbientLight::debugDescription() const
        {
            return Light::debugDescription();
        }

        PointLight::PointLight(const std::string &name)
            : Light(name)
        {
        }

        PointLight::PointLight(const PointLight &other)
            : Light(other)
        {
        }

        PointLight::~PointLight()
        {
        }

        const char *PointLight::getClassName() const
        {
            return "PointLight";
        }

        std::string PointLight::debugDescription() const
        {
            std::string str = Light::debugDescription();
            str.pop_back();
            str += ",attenuation:[" + std::to_string(_attenuation.r) + "," + std::to_string(_attenuation.g) + "," + std::to_string(_attenuation.b) + "]";
            str += ",specular:[" + std::to_string(_specular.r) + "," + std::to_string(_specular.g) + "," + std::to_string(_specular.b) + "]";
            return str + "}";
        }

        CastingLight::CastingLight(const std::string &name)
            : Light(name), _castShadow(true), _shadowClipNear(0.1f), _shadowClipFar(100.0f), _shadowMapSize(1024)
        {
        }

        CastingLight::CastingLight(const CastingLight &other)
            : Light(other), _castShadow(other._castShadow), _shadowClipNear(other._shadowClipNear), _shadowClipFar(other._shadowClipFar), _shadowMapSize(other._shadowMapSize)
        {
        }

        CastingLight::~CastingLight()
        {
        }

        const char *CastingLight::getClassName() const
        {
            return "CastingLight";
        }

        std::string CastingLight::debugDescription() const
        {
            std::string str = Light::debugDescription();
            str.pop_back();
            str += ",castShadow:" + std::to_string(_castShadow);
            str += ",shadowClipNear:" + std::to_string(_shadowClipNear);
            str += ",shadowClipFar:" + std::to_string(_shadowClipFar);
            str += ",shadowMapSize:[" + std::to_string(_shadowMapSize.x) + "," + std::to_string(_shadowMapSize.y) + "]";
            return str + "}";
        }

        bool CastingLight::isCastingShadow() const
        {
            return _castShadow;
        }

        void CastingLight::setCastingShadow(bool castShadow)
        {
            _castShadow = castShadow;
        }

        const glm::mat4 &CastingLight::getProjectionMatrix() const
        {
            return _projectionMatrix;
        }

        float CastingLight::getShadowClipNear() const
        {
            return _shadowClipNear;
        }

        void CastingLight::setShadowClipNear(float shadowClipNear)
        {
            _shadowClipNear = shadowClipNear;
            _updateProjectionMatrix();
        }

        float CastingLight::getShadowClipFar() const
        {
            return _shadowClipFar;
        }

        void CastingLight::setShadowClipFar(float shadowClipFar)
        {
            _shadowClipFar = shadowClipFar;
            _updateProjectionMatrix();
        }

        const glm::ivec2 &CastingLight::getShadowMapSize() const
        {
            return _shadowMapSize;
        }

        void CastingLight::setShadowMapSize(const glm::ivec2 &shadowMapSize)
        {
            _shadowMapSize = shadowMapSize;
        }

        const char *CastingLight::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_ORANGE;
        }

        DirectionalLight::DirectionalLight(const std::string &name)
            : CastingLight(name)
        {
        }

        DirectionalLight::DirectionalLight(const DirectionalLight &other)
            : CastingLight(other)
        {
        }

        DirectionalLight::~DirectionalLight()
        {
        }

        const char *DirectionalLight::getClassName() const
        {
            return "DirectionalLight";
        }

        std::string DirectionalLight::debugDescription() const
        {
            return CastingLight::debugDescription();
        }

        bool DirectionalLight::isInfinite() const
        {
            return _infinite;
        }

        void DirectionalLight::setInfinite(bool infinite)
        {
            _infinite = infinite;
        }

        const glm::vec2 &DirectionalLight::getShadowOrthoSize() const
        {
            return _shadowOrthoSize;
        }

        void DirectionalLight::setShadowOrthoSize(const glm::vec2 &shadowOrthoSize)
        {
            _shadowOrthoSize = shadowOrthoSize;
            _updateProjectionMatrix();
        }

        void DirectionalLight::_updateProjectionMatrix()
        {
            _projectionMatrix = glm::ortho(-_shadowOrthoSize.x / 2.0f, _shadowOrthoSize.x / 2.0f, -_shadowOrthoSize.y / 2.0f, _shadowOrthoSize.y / 2.0f, _shadowClipNear, _shadowClipFar);
        }

        SpotLight::SpotLight(const std::string &name)
            : CastingLight(name), _coneAngle(glm::radians(45.0f)), _coneAttenuation(0.0f)
        {
            _updateProjectionMatrix();
        }

        SpotLight::SpotLight(const SpotLight &other)
            : CastingLight(other), _coneAngle(other._coneAngle), _coneAttenuation(other._coneAttenuation)
        {
            _updateProjectionMatrix();
        }

        SpotLight::~SpotLight()
        {
        }

        const char *SpotLight::getClassName() const
        {
            return "SpotLight";
        }

        std::string SpotLight::debugDescription() const
        {
            std::string str = CastingLight::debugDescription();
            str.pop_back();
            str += ",coneAngle:" + std::to_string(_coneAngle);
            str += ",coneAttenuation:" + std::to_string(_coneAttenuation);
            return str + "}";
        }

        float SpotLight::getConeAngle() const
        {
            return _coneAngle;
        }

        void SpotLight::setConeAngle(float coneAngle)
        {
            _coneAngle = coneAngle;
            _updateProjectionMatrix();
        }

        float SpotLight::getConeAttenuation() const
        {
            return _coneAttenuation;
        }

        void SpotLight::setConeAttenuation(float coneAttenuation)
        {
            _coneAttenuation = coneAttenuation;
        }

        void SpotLight::_updateProjectionMatrix()
        {
            _projectionMatrix = glm::perspective(_coneAngle, 1.0f, _shadowClipNear, _shadowClipFar);
        }

    } // namespace Scene

} // namespace ST