// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene
{

    class LightNode : public PivotNode
    {
    public:
        STONE_ABSTRACT_NODE(LightNode);

        LightNode(const std::string &name = "light");
        LightNode(const LightNode &other);

        virtual ~LightNode();

        virtual std::string debugDescription() const override;

        virtual bool isCastingShadow() const;

    protected:
        float _intensity;
        glm::vec3 _color;

        virtual const char *_termClassColor() const override;
    };

    class AmbientLightNode : public LightNode
    {
    public:
        STONE_NODE(AmbientLightNode);

        AmbientLightNode(const std::string &name = "ambientlight");
        AmbientLightNode(const AmbientLightNode &other);

        virtual ~AmbientLightNode();

        virtual std::string debugDescription() const override;
    };

    class PointLightNode : public LightNode
    {
    public:
        STONE_NODE(PointLightNode);

        PointLightNode(const std::string &name = "pointlight");
        PointLightNode(const PointLightNode &other);

        virtual ~PointLightNode();

        virtual std::string debugDescription() const override;

    protected:
        glm::vec3 _attenuation;
        glm::vec3 _specular;
    };

    class CastingLightNode : public LightNode
    {
    public:
        STONE_ABSTRACT_NODE(CastingLightNode);

        CastingLightNode(const std::string &name = "castinglight");
        CastingLightNode(const CastingLightNode &other);

        virtual ~CastingLightNode();

        virtual std::string debugDescription() const override;

        virtual bool isCastingShadow() const override;
        void setCastingShadow(bool castShadow);

        const glm::mat4 &getProjectionMatrix() const;

        float getShadowClipNear() const;
        void setShadowClipNear(float shadowClipNear);

        float getShadowClipFar() const;
        void setShadowClipFar(float shadowClipFar);

        const glm::ivec2 &getShadowMapSize() const;
        void setShadowMapSize(const glm::ivec2 &shadowMapSize);

    protected:
        bool _castShadow;
        float _shadowClipNear;
        float _shadowClipFar;
        glm::ivec2 _shadowMapSize;
        glm::mat4 _projectionMatrix;

        virtual void _updateProjectionMatrix() = 0;

        virtual const char *_termClassColor() const override;
    };

    class DirectionalLightNode : public CastingLightNode
    {
    public:
        STONE_NODE(DirectionalLightNode);

        DirectionalLightNode(const std::string &name = "directionallight");
        DirectionalLightNode(const DirectionalLightNode &other);

        virtual ~DirectionalLightNode();

        virtual std::string debugDescription() const override;

        bool isInfinite() const;
        void setInfinite(bool infinite);

        const glm::vec2 &getShadowOrthoSize() const;
        void setShadowOrthoSize(const glm::vec2 &shadowOrthoSize);

    protected:
        bool _infinite;
        glm::vec2 _shadowOrthoSize;

        virtual void _updateProjectionMatrix() override;
    };

    class SpotLightNode : public CastingLightNode
    {
    public:
        STONE_NODE(SpotLightNode);

        SpotLightNode(const std::string &name = "spotlight");
        SpotLightNode(const SpotLightNode &other);

        virtual ~SpotLightNode();

        virtual std::string debugDescription() const override;

        float getConeAngle() const;
        void setConeAngle(float coneAngle);

        float getConeAttenuation() const;
        void setConeAttenuation(float coneAttenuation);

    protected:
        float _coneAngle;
        float _coneAttenuation;

        virtual void _updateProjectionMatrix() override;
    };

} // namespace Stone::Scene
