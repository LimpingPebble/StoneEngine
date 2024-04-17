// Copyright 2024 Stone-Engine

#pragma once

#include "Node.hpp"
#include "Camera.hpp"

namespace STN
{

    namespace Scene
    {

        class Scene : public Node
        {
        public:
            Scene(const std::string &name = "scene");
            Scene(const Scene &other);

            virtual ~Scene();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription(bool colored = true) const override;

            void setCamera(std::shared_ptr<Camera> camera);
            std::shared_ptr<Camera> getCamera() const;

            void render();

        protected:
            std::weak_ptr<Camera> _camera;

            virtual const char *_termClassColor() const override;
        };

    } // namespace Scene

} // namespace STN
