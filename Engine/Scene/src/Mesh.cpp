// Copyright 2024 Stone-Engine

#include "scene/Mesh.hpp"
#include "scene/ISceneRenderer.hpp"

namespace Stone
{

    namespace Scene
    {

        STONE_NODE_IMPLEMENTATION(Mesh)

        Mesh::Mesh(const std::string &name)
            : RenderableNode(name), _vertices(), _indices(), _material(nullptr)
        {
        }

        Mesh::Mesh(const Mesh &other)
            : RenderableNode(other), _vertices(other._vertices), _indices(other._indices), _material(other._material)
        {
        }

        Mesh::~Mesh()
        {
        }

        std::string Mesh::debugDescription() const
        {
            std::string str = RenderableNode::debugDescription();
            str.pop_back();
            str += ",vertices:" + std::to_string(_vertices.size());
            str += ",indices:" + std::to_string(_indices.size()) + "}";
            return str;
        }

        void Mesh::generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer)
        {
            renderer->generateDataForMesh(std::static_pointer_cast<Mesh>(shared_from_this()));
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
            return TERM_COLOR_BOLD TERM_COLOR_GREEN;
        }

        STONE_NODE_IMPLEMENTATION(InstancedMesh)

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

        std::string InstancedMesh::debugDescription() const
        {
            std::string str = Mesh::debugDescription();
            str.pop_back();
            str += ",instances:" + std::to_string(_instancesTransforms.size()) + "}";
            return str;
        }

        void InstancedMesh::generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer)
        {
            renderer->generateDataForInstancedMesh(std::static_pointer_cast<InstancedMesh>(shared_from_this()));
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

} // namespace Stone