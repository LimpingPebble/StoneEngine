// Copyright 2024 Stone-Engine

#pragma once

#include "Node.hpp"
#include "Transform.hpp"

namespace STN
{

    namespace Scene
    {
        
        class Node3D : public Node
        {
        public:
            Node3D(std::string name = "node3d");
            Node3D(const Node3D &other);

            virtual ~Node3D();

            virtual const char *getClassName() const override;

            Transform3D &getTransform();
            const Transform3D &getTransform() const;
            void setTransform(const Transform3D &transform);

            const glm::mat4& getTransformMatrix();
            const glm::mat4 getTransformMatrix() const;
            const glm::mat4 getWorldTransformMatrix();
            const glm::mat4 getWorldTransformMatrix() const;

            virtual void render(const RenderUniforms& uniforms, RenderStage stage, std::shared_ptr<Scene> scene) override;

            virtual std::string debugDescription(bool colored) const override;

        protected:
            Transform3D _transform;

            virtual const char *_termClassColor() const override;
        };

    } // namespace Scene

} // namespace STN
