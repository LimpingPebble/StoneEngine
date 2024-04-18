// Copyright 2024 Stone-Engine

#include "Skeleton.hpp"
#include "Pivot.hpp"

namespace STN
{

    namespace Scene
    {

        Skeleton::Bone::Bone(std::shared_ptr<Pivot> pivot)
            : pivot(pivot), inverseBindMatrix(1.0f), restPose(pivot->getTransform())
        {
        }

        Skeleton::Skeleton(const std::string &name)
            : Node(name), _bones()
        {
        }

        Skeleton::Skeleton(const Skeleton &other)
            : Node(other), _bones(other._bones)
        {
        }

        Skeleton::~Skeleton()
        {
        }

        const char *Skeleton::getClassName() const
        {
            return "Skeleton";
        }

        std::string Skeleton::debugDescription(bool colored) const
        {
            std::string str = Node::debugDescription(colored);
            str.pop_back();
            str += "bones:[";
            for (const auto &bone : _bones)
            {
                if (auto pivot = bone.pivot.lock())
                {
                    str += pivot->getGlobalName() + ",";
                }
            }
            str.pop_back();
            str += "]}";
            return str;
        }

        const std::vector<Skeleton::Bone> &Skeleton::getBones() const
        {
            return _bones;
        }

        void Skeleton::addBone(std::shared_ptr<Pivot> pivot)
        {
            Bone bone(pivot);
            if (!_bones.empty())
            {
                bone.inverseBindMatrix = glm::inverse(pivot->getTransformMatrixRelativeToNode(_bones[0].pivot.lock()));
            }
            _bones.push_back(Bone(pivot));
        }

        void Skeleton::addBone(std::shared_ptr<Pivot> pivot, glm::mat4 offset)
        {
            Bone bone(pivot);
            bone.inverseBindMatrix = offset;
            _bones.push_back(bone);
        }

        const char *Skeleton::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_CYAN;
        }

    } // namespace Scene

} // namespace STN