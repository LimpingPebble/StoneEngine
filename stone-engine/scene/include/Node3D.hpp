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
            Node3D();
            Node3D(const Node3D &other);

            virtual ~Node3D();

            Transform3D &getTransform();
            const Transform3D &getTransform() const;

            virtual void update(float deltaTime);
            virtual void render(RenderUniforms &uniforms, RenderStage stage, std::shared_ptr<Scene> scene);

        protected:
            Transform3D _transform;
        };

    } // namespace Scene

} // namespace STN