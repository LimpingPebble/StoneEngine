// Copyright 2024 Stone-Engine

#include "Mesh.hpp"

namespace STN
{

    namespace Scene
    {

        Mesh::Mesh(const std::string &name)
            : Node(name), _vertices(), _indices(), _material(nullptr)
        {
        }

        Mesh::Mesh(const Mesh &other)
            : Node(other), _vertices(other._vertices), _indices(other._indices), _material(other._material)
        {
        }

        Mesh::~Mesh()
        {
        }

        const char *Mesh::getClassName() const
        {
            return "Mesh";
        }

        std::string Mesh::debugDescription(bool colored) const
        {
            std::string str = Node::debugDescription(colored);
            str += "{vertices:" + std::to_string(_vertices.size());
            str += ",indices:" + std::to_string(_indices.size()) + "}";
            return str;
        }

        const std::vector<Vertex> &Mesh::getVertices() const
        {
            return _vertices;
        }

        const std::vector<uint32_t> &Mesh::getIndices() const
        {
            return _indices;
        }

        std::vector<Vertex> &Mesh::verticesRef()
        {
            return _vertices;
        }

        std::vector<uint32_t> &Mesh::indicesRef()
        {
            return _indices;
        }

        std::shared_ptr<Material> Mesh::getMaterial() const
        {
            return _material;
        }

        void Mesh::setMaterial(std::shared_ptr<Material> material)
        {
            _material = material;
        }

        const char *Mesh::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_CYAN;
        }

        InstancedMesh::InstancedMesh(const std::string &name)
        : Mesh(name), _instancesTransforms()
        {
        }

        InstancedMesh::InstancedMesh(const InstancedMesh &other)
        : Mesh(other), _instancesTransforms(other._instancesTransforms)
        {
        }

        InstancedMesh::~InstancedMesh()
        {
        }

        const char *InstancedMesh::getClassName() const
        {
            return "InstancedMesh";
        }

        std::string InstancedMesh::debugDescription(bool colored) const
        {
            std::string str = Mesh::debugDescription(colored);
            str.pop_back();
            str += ",instances:" + std::to_string(_instancesTransforms.size()) + "}";
            return str;
        }

        void InstancedMesh::addInstance(const Transform3D &transform)
        {
            _instancesTransforms.push_back(transform);
        }

        void InstancedMesh::removeInstance(size_t index)
        {
            assert(index < _instancesTransforms.size());
            _instancesTransforms.erase(_instancesTransforms.begin() + index);
        }

        void InstancedMesh::clearInstances()
        {
            _instancesTransforms.clear();
        }

        const std::vector<Transform3D> &InstancedMesh::getInstancesTransforms() const
        {
            return _instancesTransforms;
        }

    } // namespace Scene

} // namespace STN