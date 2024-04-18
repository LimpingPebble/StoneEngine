// Copyright 2024 Stone-Engine

#pragma once

#include "Node.hpp"
#include "Transform.hpp"

namespace STN
{

    namespace Scene
    {

        class Pivot : public Node
        {
        public:
            Pivot(const std::string &name = "pivot");
            Pivot(const Pivot &other);

            virtual ~Pivot();

            virtual const char *getClassName() const override;
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

} // namespace STN
