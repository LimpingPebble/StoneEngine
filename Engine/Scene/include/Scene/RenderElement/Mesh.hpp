// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/RenderElement/IRenderElement.hpp"
#include "Scene/Vertex.hpp"
#include <vector>

namespace Stone::Scene
{

    class Mesh : public Core::Object, public IRenderElement
    {
    public:
        Mesh();
        Mesh(const Mesh &other);

        virtual ~Mesh();

        virtual std::string debugDescription() const override;

        virtual void generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer);

        const std::vector<Vertex> &getVertices() const;
        const std::vector<uint32_t> &getIndices() const;
        std::vector<Vertex> &verticesRef();
        std::vector<uint32_t> &indicesRef();

    protected:
        std::vector<Vertex> _vertices;
        std::vector<uint32_t> _indices;
    };

} // namespace Stone::Scene
