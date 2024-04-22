// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node.hpp"
#include "Scene/Camera.hpp"

namespace Stone
{

    namespace Scene
    {

        class Scene : public Node
        {
        public:
            STONE_NODE(Scene);

            Scene(const std::string &name = "scene");
            Scene(const Scene &other);

            virtual ~Scene();

            virtual std::string debugDescription() const override;

            void setCamera(std::shared_ptr<Camera> camera);
            std::shared_ptr<Camera> getCamera() const;

            void render();

        protected:
            std::weak_ptr<Camera> _camera;

            virtual const char *_termClassColor() const override;
        };

    } // namespace Scene

} // namespace Stone
