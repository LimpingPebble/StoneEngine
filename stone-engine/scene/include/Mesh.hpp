// Copyright 2024 Stone-Engine

#pragma once

#include "Node.hpp"
#include "Transform.hpp"
#include "RenderTypes.hpp"

namespace STN
{

    namespace Scene
    {

        class Material;

        class Mesh : public Node
        {
        public:
            Mesh(const std::string &name = "mesh");
            Mesh(const Mesh &other);

            virtual ~Mesh();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription(bool colored = true) const override;

            const std::vector<Vertex> &getVertices() const;
            const std::vector<uint32_t> &getIndices() const;
            std::vector<Vertex> &verticesRef();
            std::vector<uint32_t> &indicesRef();

            std::shared_ptr<Material> getMaterial() const;
            void setMaterial(std::shared_ptr<Material> material);

        protected:
            std::vector<Vertex> _vertices;
            std::vector<uint32_t> _indices;
            std::shared_ptr<Material> _material;

            virtual const char *_termClassColor() const override;
        };

        class InstancedMesh : public Mesh
        {
        public:
            InstancedMesh(const std::string &name = "instanced_mesh");
            InstancedMesh(const InstancedMesh &other);

            virtual ~InstancedMesh();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription(bool colored = true) const override;

            void addInstance(const Transform3D &transform);
            void removeInstance(size_t index);
            void clearInstances();

            const std::vector<Transform3D> &getInstancesTransforms() const;

        protected:
            std::vector<Transform3D> _instancesTransforms;
        };

    } // namespace Scene

} // namespace STN
