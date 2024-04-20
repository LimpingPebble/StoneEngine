// Copyright 2024 Stone-Engine

#include "scene/SkinnedMesh.hpp"
#include "scene/ISceneRenderer.hpp"
#include "scene/Skeleton.hpp"

namespace Stone
{

    namespace Scene
    {

        SkinnedMesh::SkinnedMesh(const std::string &name)
            : RenderableNode(name), _vertices(), _indices(), _skeleton()
        {
        }

        SkinnedMesh::SkinnedMesh(const SkinnedMesh &other)
            : RenderableNode(other), _vertices(other._vertices), _indices(other._indices), _skeleton(other._skeleton)
        {
        }

        SkinnedMesh::~SkinnedMesh()
        {
        }

        const char *SkinnedMesh::getClassName() const
        {
            return "SkinnedMesh";
        }

        std::string SkinnedMesh::debugDescription() const
        {
            std::string str = RenderableNode::debugDescription();
            str.pop_back();
            str += ",vertices:" + std::to_string(_vertices.size());
            str += ",indices:" + std::to_string(_indices.size());
            str += ",skeleton:" + (_skeleton.expired() ? "nullptr" : _skeleton.lock()->getGlobalName()) + "}";
            return str;
        }

        void SkinnedMesh::generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer)
        {
            renderer->generateDataForSkinnedMesh(std::static_pointer_cast<SkinnedMesh>(shared_from_this()));
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

} // namespace Stone