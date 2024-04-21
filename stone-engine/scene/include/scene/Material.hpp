// Copyright 2024 Stone-Engine

#pragma once

#include "core/Object.hpp"
#include "scene/IRenderElement.hpp"

namespace Stone
{

    namespace Scene
    {

        class Texture;

        class Material : public Core::Object, public IRenderElement
        {
        public:
            Material();
            Material(const Material &other);

            virtual ~Material();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            virtual void generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer) override;
        };

        class PBRMaterial : public Material
        {
        public:
            PBRMaterial();
            PBRMaterial(const PBRMaterial &other);

            virtual ~PBRMaterial();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            virtual void generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer) override;

            std::shared_ptr<Texture> albedo;
            std::shared_ptr<Texture> normal;
            std::shared_ptr<Texture> metallic;
            std::shared_ptr<Texture> roughness;
            std::shared_ptr<Texture> occlusion;
        };

        class ShaderMaterial : public Material
        {
        public:
            ShaderMaterial(std::string shader_file);
            ShaderMaterial(const ShaderMaterial &other);

            virtual ~ShaderMaterial();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            virtual void generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer) override;
        };

    } // namespace Scene

} // namespace Stone
