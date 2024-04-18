// Copyright 2024 Stone-Engine

#pragma once

#include "Node.hpp"
#include "Transform.hpp"

namespace STN
{

    namespace Scene
    {

        class Pivot;

        class Skeleton : public Node
        {
        public:
            struct Bone
            {
                std::weak_ptr<Pivot> pivot;
                glm::mat4 inverseBindMatrix;
                Transform3D restPose;

                Bone(std::shared_ptr<Pivot> pivot);
            };

            Skeleton(const std::string &name = "skeleton");
            Skeleton(const Skeleton &other);

            virtual ~Skeleton();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            const std::vector<Bone> &getBones() const;
            void addBone(std::shared_ptr<Pivot> pivot);
            void addBone(std::shared_ptr<Pivot> pivot, glm::mat4 offset);

        protected:
            std::vector<Bone> _bones;

            virtual const char *_termClassColor() const override;
        };

    } // namespace Scene

} // namespace STN
