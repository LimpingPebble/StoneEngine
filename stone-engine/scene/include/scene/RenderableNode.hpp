// Copyright 2024 Stone-Engine

#pragma once

#include "scene/Node.hpp"
#include "scene/IRenderElement.hpp"

namespace Stone
{

    namespace Scene
    {

        class RenderableNode : public Node, public IRenderElement
        {
        public:
            RenderableNode(const std::string &name = "renderable_node");
            RenderableNode(const RenderableNode &other);

            virtual ~RenderableNode();

            virtual std::string debugDescription() const override;

            virtual void render(RenderContext &context);
        };

    } // namespace Scene

} // namespace Stone
