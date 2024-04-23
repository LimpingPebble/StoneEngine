// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/Node.hpp"
#include "Scene/Transform.hpp"

namespace Stone::Scene
{

    class PivotNode : public Node
    {
    public:
        STONE_NODE(PivotNode);

        PivotNode(const std::string &name = "pivot");
        PivotNode(const PivotNode &other);

        virtual ~PivotNode();

        virtual std::string debugDescription() const override;

        virtual void render(RenderContext &context) override;

        virtual void transformRelativeMatrix(glm::mat4 &relative) const override;

        Transform3D &getTransform();
        const Transform3D &getTransform() const;
        void setTransform(const Transform3D &transform);

        const glm::mat4 &getTransformMatrix();
        const glm::mat4 getTransformMatrix() const;

    protected:
        Transform3D _transform;

        virtual const char *_termClassColor() const override;
    };

} // namespace Stone::Scene
