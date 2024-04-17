// Copyright 2024 Stone-Engine

#pragma once

#include "Node3D.hpp"

namespace STN
{

    namespace Scene
    {

        class Light : public Node3D
        {
        public:
            Light();
            Light(const Light &other);

            virtual ~Light();

            /** Interested in PreProcessing stage to list all lights all process shadows */
            virtual void render(RenderUniforms &uniforms, RenderStage stage, std::shared_ptr<Scene> scene) override;

            virtual bool isCastingShadow() const { return false; }

        protected:
            float _intensity;
            glm::vec3 _color;
        };

        class AmbientLight : public Light
        {
        public:
            AmbientLight();
            AmbientLight(const AmbientLight &other);

            virtual ~AmbientLight();
        };

        class PointLight : public Light
        {
        public:
            PointLight();
            PointLight(const PointLight &other);

            virtual ~PointLight();

        protected:
            glm::vec3 _attenuation;
            glm::vec3 _specular;
        };

        class CastingLight : public Light
        {
        public:
            bool isCastingShadow() const override { return _castShadow; }

        protected:
            bool _castShadow;
            float _shadowClipNear;
            float _shadowClipFar;
            glm::ivec2 _shadowMapSize;
        };

        class DirectionalLight : public CastingLight
        {
        public:
            DirectionalLight();
            DirectionalLight(const DirectionalLight &other);

            virtual ~DirectionalLight();
        };

        class SpotLight : public CastingLight
        {
        public:
            SpotLight();
            SpotLight(const SpotLight &other);

            virtual ~SpotLight();

        protected:
            float _coneAngle;
            float _coneAttenuation;
        };

    } // namespace Scene

} // namespace STN
