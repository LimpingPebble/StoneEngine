// Copyright 2024 Stone-Engine

#pragma once

#include "node.hpp"
#include "transform.hpp"

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

            virtual void update(float deltaTime) override;
            virtual void render() override;

        protected:
            Transform3D _transform;
        };

    } // namespace Scene

} // namespace STN
