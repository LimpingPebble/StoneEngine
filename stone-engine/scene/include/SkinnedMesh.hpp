// Copyright 2024 Stone-Engine

#pragma once

#include "Node3D.hpp"

namespace STN
{

    namespace Scene
    {

        class Skeleton;

        class SkinnedMesh : public Node
        {
        public:
            SkinnedMesh();
            SkinnedMesh(const SkinnedMesh &other);

            virtual ~SkinnedMesh();

            void setSkeleton(std::shared_ptr<Skeleton> skeleton);
            std::shared_ptr<Skeleton> getSkeleton() const;

        protected:
            std::shared_ptr<Skeleton> _skeleton;
        };

    } // namespace Scene

} // namespace STN
