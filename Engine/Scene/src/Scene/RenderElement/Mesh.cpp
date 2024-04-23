// Copyright 2024 Stone-Engine

#include "Scene/RenderElement/Mesh.hpp"
#include "Scene/ISceneRenderer.hpp"

namespace Stone::Scene
{

    Mesh::Mesh()
        : Object(), IRenderElement(), _vertices(), _indices()
    {
    }

    Mesh::Mesh(const Mesh &other)
        : Object(other), IRenderElement(other), _vertices(other._vertices), _indices(other._indices)
    {
    }

    Mesh::~Mesh()
    {
    }

    std::string Mesh::debugDescription() const
    {
        std::string str = Object::debugDescription();
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

} // namespace Stone::Scene
