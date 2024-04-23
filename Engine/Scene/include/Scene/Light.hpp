// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node.hpp"

namespace Stone
{

    namespace Scene
    {

        class Light : public Node
        {
        public:
            STONE_ABSTRACT_NODE(Light);

            Light(const std::string &name = "light");
            Light(const Light &other);

            virtual ~Light();

            virtual std::string debugDescription() const override;

            virtual bool isCastingShadow() const;

        protected:
            float _intensity;
            glm::vec3 _color;

            virtual const char *_termClassColor() const override;
        };

        class AmbientLight : public Light
        {
        public:
            STONE_NODE(AmbientLight);

            AmbientLight(const std::string &name = "ambient_light");
            AmbientLight(const AmbientLight &other);

            virtual ~AmbientLight();

            virtual std::string debugDescription() const override;
        };

        class PointLight : public Light
        {
        public:
            STONE_NODE(PointLight);

            PointLight(const std::string &name = "point_light");
            PointLight(const PointLight &other);

            virtual ~PointLight();

            virtual std::string debugDescription() const override;

        protected:
            glm::vec3 _attenuation;
            glm::vec3 _specular;
        };

        class CastingLight : public Light
        {
        public:
            STONE_ABSTRACT_NODE(CastingLight);

            CastingLight(const std::string &name = "casting_light");
            CastingLight(const CastingLight &other);

            virtual ~CastingLight();

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

        class DirectionalLight : public CastingLight
        {
        public:
            STONE_NODE(DirectionalLight);

            DirectionalLight(const std::string &name = "directional_light");
            DirectionalLight(const DirectionalLight &other);

            virtual ~DirectionalLight();

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

        class SpotLight : public CastingLight
        {
        public:
            STONE_NODE(SpotLight);

            SpotLight(const std::string &name = "spot_light");
            SpotLight(const SpotLight &other);

            virtual ~SpotLight();

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

    } // namespace Scene

} // namespace Stone
