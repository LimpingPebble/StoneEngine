// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/Node.hpp"

namespace Stone::Scene
{

    class CameraNode;

    class WorldNode : public Node
    {
    public:
        STONE_NODE(WorldNode);

        WorldNode(const std::string &name = "world");
        WorldNode(const WorldNode &other);

        virtual ~WorldNode();

        virtual std::ostream &writeToStream(std::ostream &stream, bool closing_bracer = true) const override;

        void setActiveCamera(std::shared_ptr<CameraNode> camera);
        std::shared_ptr<CameraNode> getActiveCamera() const;

        void render();

    protected:
        std::weak_ptr<CameraNode> _activeCamera;

        virtual const char *_termClassColor() const override;
    };

} // namespace Stone::Scene
