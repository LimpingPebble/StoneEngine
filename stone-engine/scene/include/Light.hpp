// Copyright 2024 Stone-Engine

#pragma once

#include "Node.hpp"

namespace STN
{

    namespace Scene
    {

        class Light : public Node
        {
        public:
            Light(const std::string &name = "light");
            Light(const Light &other);

            virtual ~Light();

            virtual const char *getClassName() const override;

            virtual bool isCastingShadow() const;
            virtual std::string debugDescription() const override;

        protected:
            float _intensity;
            glm::vec3 _color;

            virtual const char *_termClassColor() const override;
        };

        class AmbientLight : public Light
        {
        public:
            AmbientLight(const std::string &name = "ambient_light");
            AmbientLight(const AmbientLight &other);

            virtual ~AmbientLight();

            virtual const char *getClassName() const override;

            virtual std::string debugDescription() const override;
        };

        class PointLight : public Light
        {
        public:
            PointLight(const std::string &name = "point_light");
            PointLight(const PointLight &other);

            virtual ~PointLight();

            virtual const char *getClassName() const override;

            virtual std::string debugDescription() const override;

        protected:
            glm::vec3 _attenuation;
            glm::vec3 _specular;
        };

        class CastingLight : public Light
        {
        public:
            CastingLight(const std::string &name = "casting_light");
            CastingLight(const CastingLight &other);

            virtual ~CastingLight();

            virtual const char *getClassName() const override;
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
            DirectionalLight(const std::string &name = "directional_light");
            DirectionalLight(const DirectionalLight &other);

            virtual ~DirectionalLight();

            virtual const char *getClassName() const override;
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
            SpotLight(const std::string &name = "spot_light");
            SpotLight(const SpotLight &other);

            virtual ~SpotLight();

            virtual const char *getClassName() const override;
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

} // namespace STN
