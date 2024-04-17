// Copyright 2024 Stone-Engine

#pragma once

#include "Node.hpp"

namespace STN
{

    namespace Scene
    {

        class Skybox : public Node
        {
        public:
            Skybox();
            Skybox(const Skybox &other);

            virtual ~Skybox();

            virtual void render(RenderUniforms &uniforms, RenderStage stage, std::shared_ptr<Scene> scene) override;
        };

    } // namespace Scene

} // namespace STN
