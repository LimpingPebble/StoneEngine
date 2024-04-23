// Copyright 2024 Stone-Engine

#include "Scene/Node/SkeletonNode.hpp"
#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene
{

    STONE_NODE_IMPLEMENTATION(SkeletonNode)

    SkeletonNode::Bone::Bone(std::shared_ptr<PivotNode> pivot)
        : pivot(pivot), inverseBindMatrix(1.0f), restPose(pivot->getTransform())
    {
    }

    SkeletonNode::SkeletonNode(const std::string &name)
        : Node(name), _bones()
    {
    }

    SkeletonNode::SkeletonNode(const SkeletonNode &other)
        : Node(other), _bones(other._bones)
    {
    }

    SkeletonNode::~SkeletonNode()
    {
    }

    std::ostream &SkeletonNode::writeToStream(std::ostream &stream, bool closing_bracer) const
    {
        Node::writeToStream(stream, false);
        stream << ",bones:[";
        for (const auto &bone : _bones)
        {
            stream << bone.pivot.lock()->getGlobalName() << ",";
        }
        stream << "]";
        if (closing_bracer)
            stream << "}";
        return stream;
    }

    const std::vector<SkeletonNode::Bone> &SkeletonNode::getBones() const
    {
        return _bones;
    }

    void SkeletonNode::addBone(std::shared_ptr<PivotNode> pivot)
    {
        Bone bone(pivot);
        if (!_bones.empty())
        {
            bone.inverseBindMatrix = glm::inverse(pivot->getTransformMatrixRelativeToNode(_bones[0].pivot.lock()));
        }
        _bones.push_back(Bone(pivot));
    }

    void SkeletonNode::addBone(std::shared_ptr<PivotNode> pivot, glm::mat4 offset)
    {
        Bone bone(pivot);
        bone.inverseBindMatrix = offset;
        _bones.push_back(bone);
    }

    const char *SkeletonNode::_termClassColor() const
    {
        return TERM_COLOR_BOLD TERM_COLOR_CYAN;
    }

} // namespace Scene::Stone
