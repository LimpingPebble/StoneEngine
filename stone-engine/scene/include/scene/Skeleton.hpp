// Copyright 2024 Stone-Engine

#pragma once

#include "scene/Node.hpp"
#include "scene/Transform.hpp"

namespace Stone
{

    namespace Scene
    {

        class Pivot;

        class Skeleton : public Node
        {
        public:
            STONE_NODE(Skeleton);

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

            virtual std::string debugDescription() const override;

            const std::vector<Bone> &getBones() const;
            void addBone(std::shared_ptr<Pivot> pivot);
            void addBone(std::shared_ptr<Pivot> pivot, glm::mat4 offset);

        protected:
            std::vector<Bone> _bones;

            virtual const char *_termClassColor() const override;
        };

    } // namespace Scene

} // namespace Stone
