// Copyright 2024 Stone-Engine

#pragma once

#include "Node3D.hpp"

namespace STN
{

    namespace Scene
    {

        class Skeleton : public Node3D
        {
        public:

            struct Bone {
                std::weak_ptr<Node3D> node;
                glm::mat4 inverseBindMatrix;
                Transform3D restPose;

                Bone(std::shared_ptr<Node3D> node);
            }

            Skeleton();
            Skeleton(const Skeleton &other);

            virtual ~Skeleton();

        protected:
            std::vector<Bone> _bones;

        };

    } // namespace Scene

} // namespace STN
