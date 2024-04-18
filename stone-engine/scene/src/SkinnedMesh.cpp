// Copyright 2024 Stone-Engine

#include "SkinnedMesh.hpp"
#include "Pivot.hpp"
#include "Skeleton.hpp"

namespace STN
{

    namespace Scene
    {

        SkinnedMesh::SkinnedMesh(const std::string &name)
        : Node(name), _vertices(), _indices()
        {
        }

        SkinnedMesh::SkinnedMesh(const SkinnedMesh &other)
        : Node(other), _vertices(), _indices(), _skeleton(other._skeleton)
        {
        }

        SkinnedMesh::~SkinnedMesh()
        {
        }

        const char *SkinnedMesh::getClassName() const
        {
            return "SkinnedMesh";
        }

        std::string SkinnedMesh::debugDescription(bool colored) const
        {
            std::string str = Node::debugDescription(colored);
            str.pop_back();
            str += "vertices:" + std::to_string(_vertices.size()) + ",";
            str += "indices:" + std::to_string(_indices.size()) + ",";
            str += "skeleton:" + (_skeleton.expired() ? "null" : _skeleton.lock()->getGlobalName()) + "}";
            return str;
        }

        const std::vector<WeightVertex> &SkinnedMesh::getVertices() const
        {
            return _vertices;
        }

        const std::vector<uint32_t> &SkinnedMesh::getIndices() const
        {
            return _indices;
        }

        std::vector<WeightVertex> &SkinnedMesh::verticesRef()
        {
            return _vertices;
        }

        std::vector<uint32_t> &SkinnedMesh::indicesRef()
        {
            return _indices;
        }

        void SkinnedMesh::setSkeleton(std::shared_ptr<Skeleton> skeleton)
        {
            _skeleton = skeleton;
        }

        std::shared_ptr<Skeleton> SkinnedMesh::getSkeleton() const
        {
            return _skeleton.lock();
        }

        const char *SkinnedMesh::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_MAGENTA;
        }

    } // namespace Scene

} // namespace STN