// Copyright 2024 Stone-Engine

#include "scene/RenderableNode.hpp"

namespace Stone
{

    namespace Scene
    {

        STONE_ABSTRACT_NODE_IMPLEMENTATION(RenderableNode)

        RenderableNode::RenderableNode(const std::string &name)
            : Node(name), IRenderElement()
        {
        }

        RenderableNode::RenderableNode(const RenderableNode &other)
            : Node(other), IRenderElement(other)
        {
        }

        RenderableNode::~RenderableNode()
        {
        }

        std::string RenderableNode::debugDescription() const
        {
            std::string str = Node::debugDescription();
            str.pop_back();
            str += ",dirty:" + std::to_string(_dirty) + "}";
            return str;
        }

        void RenderableNode::render(RenderContext &context)
        {
            if (_renderBehaviour)
            {
                _renderBehaviour->render(context);
            }
        }

    } // namespace Scene

} // namespace Stone