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

    } // namespace Scene

} // namespace Stone
