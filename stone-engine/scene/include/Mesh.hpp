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
            Mesh();
            Mesh(const Mesh &other);

            virtual ~Mesh();

            std::shared_ptr<Material> getMaterial() const;

        protected:
            std::shared_ptr<Material> _material;
        };

        class InstancedMesh : public Mesh
        {
            public:

                InstancedMesh();
                InstancedMesh(const InstancedMesh &other);

                virtual ~InstancedMesh();

                void addInstance(const Transform3D &transform);
                void removeInstance(int index);
                void clearInstances();

                const std::vector<Transform3D> &getInstancesTransforms() const;

            protected:
                std::vector<Transform3D> _instancesTransforms;
        };

    } // namespace Scene

} // namespace STN
