// Copyright 2024 Stone-Engine

#pragma once

#include <glm/mat4x4.hpp>
#include <memory>

namespace Stone
{

    namespace Scene
    {

        enum class RenderStage
        {
            /** Render without color when calculating casted shadows */
            Depth,
            /** Render before every other elements. Usefull to render fog, lights, etc... */
            PreProcessing,
            /** Render objects with fully opaque colors */
            Opaque,
            /** Render objects with transparent materials after opaques are rendered */
            Transparent,
            /** Custom Stencil render */
            CustomStencil,
            /** Render post processing effects */
            PostProcessing,
        };

        struct RenderContext
        {
            glm::mat4 projectionMatrix = glm::mat4(1.0f);
            glm::mat4 viewMatrix = glm::mat4(1.0f);
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            RenderStage stage = RenderStage::Opaque;
            void *graphicsContext = nullptr;
        };

    } // namespace Scene

} // namespace Stone
