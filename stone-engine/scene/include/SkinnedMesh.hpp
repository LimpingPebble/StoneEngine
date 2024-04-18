// Copyright 2024 Stone-Engine

#pragma once

#include "Node.hpp"
#include "RenderTypes.hpp"

namespace STN
{

    namespace Scene
    {

        class Skeleton;
        class Pivot;

        class SkinnedMesh : public Node
        {
        public:
            SkinnedMesh(const std::string &name = "skinned_mesh");
            SkinnedMesh(const SkinnedMesh &other);

            virtual ~SkinnedMesh();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription(bool colored = true) const override;

            const std::vector<WeightVertex> &getVertices() const;
            const std::vector<uint32_t> &getIndices() const;
            std::vector<WeightVertex> &verticesRef();
            std::vector<uint32_t> &indicesRef();

            void setSkeleton(std::shared_ptr<Skeleton> skeleton);
            std::shared_ptr<Skeleton> getSkeleton() const;

        protected:
            std::vector<WeightVertex> _vertices;
            std::vector<uint32_t> _indices;
            std::weak_ptr<Skeleton> _skeleton;

            virtual const char *_termClassColor() const override;
        };

    } // namespace Scene

} // namespace STN
