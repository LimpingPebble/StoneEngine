// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene
{

    class Mesh;
    class Material;

    class MeshNode : public PivotNode
    {
    public:
        STONE_NODE(MeshNode);

        MeshNode(const std::string &name = "mesh");
        MeshNode(const MeshNode &other);

        virtual ~MeshNode();

        virtual std::ostream &writeToStream(std::ostream &stream, bool closing_bracer = true) const override;

        std::shared_ptr<Mesh> getMesh() const;
        void setMesh(std::shared_ptr<Mesh> mesh);

        std::shared_ptr<Material> getMaterial() const;
        void setMaterial(std::shared_ptr<Material> material);

    protected:
        std::shared_ptr<Mesh> _mesh;
        std::shared_ptr<Material> _material;

        virtual const char *_termClassColor() const override;
    };

    class InstancedMeshNode : public MeshNode
    {
    public:
        STONE_NODE(InstancedMeshNode);

        InstancedMeshNode(const std::string &name = "instancedmesh");
        InstancedMeshNode(const InstancedMeshNode &other);

        virtual ~InstancedMeshNode();

        virtual std::ostream &writeToStream(std::ostream &stream, bool closing_bracer = true) const override;

        void addInstance(const Transform3D &transform);
        void removeInstance(size_t index);
        void clearInstances();

        const std::vector<Transform3D> &getInstancesTransforms() const;

    protected:
        std::vector<Transform3D> _instancesTransforms;
    };

} // namespace Stone::Scene
