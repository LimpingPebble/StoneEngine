// Copyright 2024 Stone-Engine

#pragma once

#include "scene/Node.hpp"
#include "scene/Transform.hpp"

namespace Stone
{

    namespace Scene
    {

        class Pivot : public Node
        {
        public:
            STONE_NODE(Pivot);

            Pivot(const std::string &name = "pivot");
            Pivot(const Pivot &other);

            virtual ~Pivot();

            virtual std::string debugDescription() const override;

            Transform3D &getTransform();
            const Transform3D &getTransform() const;
            void setTransform(const Transform3D &transform);

            virtual void transformRelativeMatrix(glm::mat4 &relative) const override;

            const glm::mat4 &getTransformMatrix();
            const glm::mat4 getTransformMatrix() const;

            virtual void render(RenderContext &context) override;


        protected:
            Transform3D _transform;

            virtual const char *_termClassColor() const override;
        };

    } // namespace Scene

} // namespace Stone