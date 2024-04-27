// Copyright 2024 Stone-Engine

#include "Scene/Node/SkinMeshNode.hpp"
#include "Scene/RenderElement/SkinMesh.hpp"
#include "Scene/RenderElement/Material.hpp"
#include "Scene/Node/SkeletonNode.hpp"

namespace Stone::Scene
{

    STONE_NODE_IMPLEMENTATION(SkinMeshNode)

    SkinMeshNode::SkinMeshNode(const std::string &name)
        : PivotNode(name), _mesh(), _material(), _skeleton()
    {
    }

    SkinMeshNode::SkinMeshNode(const SkinMeshNode &other)
        : PivotNode(other), _mesh(other._mesh),  _material(other._material), _skeleton(other._skeleton)
    {
    }

    SkinMeshNode::~SkinMeshNode()
    {
    }

    std::ostream &SkinMeshNode::writeToStream(std::ostream &stream, bool closing_bracer) const
    {
        PivotNode::writeToStream(stream, false);
        stream << ",mesh:" << (_mesh == nullptr ? "null" : std::to_string(_mesh->getId()));
        stream << ",material:" << (_material == nullptr ? "null" : std::to_string(_material->getId()));
        stream << ",skeleton:" << (_skeleton.expired() ? "null" : _skeleton.lock()->getGlobalName());
        if (closing_bracer)
            stream << "}";
        return stream;
    }

    std::shared_ptr<SkinMesh> SkinMeshNode::getSkinMesh() const
    {
        return _mesh;
    }

    void SkinMeshNode::setSkinMesh(std::shared_ptr<SkinMesh> mesh)
    {
        _mesh = mesh;
    }

    std::shared_ptr<Material> SkinMeshNode::getMaterial() const
    {
        return _material;
    }

    void SkinMeshNode::setMaterial(std::shared_ptr<Material> material)
    {
        _material = material;
    }

    std::shared_ptr<SkeletonNode> SkinMeshNode::getSkeleton() const
    {
        return _skeleton.lock();
    }

    void SkinMeshNode::setSkeleton(std::shared_ptr<SkeletonNode> skeleton)
    {
        _skeleton = skeleton;
    }

    const char *SkinMeshNode::_termClassColor() const
    {
        return TERM_COLOR_BOLD TERM_COLOR_MAGENTA;
    }

} // namespace Scene::Stone
