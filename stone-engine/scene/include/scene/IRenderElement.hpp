// Copyright 2024 Stone-Engine

#pragma once

#include "scene/RenderContext.hpp"
#include "scene/Node.hpp"

namespace Stone
{

    namespace Scene
    {
        /**
         * @brief Interface for render behaviour
         *
         * This interface is used to define the render behaviour of a renderable element.
         * The render behaviour is the way the element is rendered in the scene.
         *
         * ```
         * class VulkanMesh : IRenderBehaviour
         * {
         * public:
         *    VulkanMesh(std::shared_ptr<Mesh> mesh);
         *    virtual void render(RenderContext &context) override;
         * }
         * ```
         */
        class IRenderBehaviour
        {
        public:
            virtual void render(RenderContext &context) = 0;
        };

        class ISceneRenderer;

        class IRenderElement
        {
        public:
            IRenderElement()
                : _renderBehaviour(nullptr), _dirty(true) {}
            IRenderElement(const IRenderElement &other)
                : _renderBehaviour(other._renderBehaviour), _dirty(true) {}

            virtual ~IRenderElement() {}

            virtual void generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer) = 0;

            bool isDirty() const { return _dirty; }
            void markDirty() { _dirty = true; }

        protected:
            friend class ISceneRenderer;

            void markUndirty() { _dirty = false; }

            std::shared_ptr<IRenderBehaviour> _renderBehaviour;
            bool _dirty;
        };

    } // namespace Scene

} // namespace Stone
