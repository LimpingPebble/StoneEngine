// Copyright 2024 Stone-Engine

#pragma once

#include "Node3D.hpp"
#include "Camera.hpp"

namespace STN
{

    namespace Scene
    {

        class Scene : public Node3D
        {
        public:
            Scene();
            Scene(const Scene &other);
            virtual ~Scene();

        protected:
            std::weak_ptr<Camera> _camera;
        };

    } // namespace Scene

} // namespace STN
