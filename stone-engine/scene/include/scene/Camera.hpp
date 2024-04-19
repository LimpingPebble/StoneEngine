// Copyright 2024 Stone-Engine

#pragma once

#include "scene/Node.hpp"

namespace Stone
{

    namespace Scene
    {

        class Camera : public Node
        {
        public:
            Camera(const std::string &name = "camera");
            Camera(const Camera &other);

            virtual ~Camera();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            virtual const glm::mat4 getProjectionMatrix() const = 0;

            float getNear() const;
            void setNear(float near);

            float getFar() const;
            void setFar(float far);

        protected:
            float _near;
            float _far;

            virtual const char *_termClassColor() const override;
        };

        class PerspectiveCamera : public Camera
        {
        public:
            PerspectiveCamera(const std::string &name = "perspective_camera");
            PerspectiveCamera(const PerspectiveCamera &other);
            virtual ~PerspectiveCamera();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            virtual const glm::mat4 getProjectionMatrix() const override;

            float getFov() const;
            void setFov(float fov);

            float getAspect() const;
            void setAspect(float aspect);

        protected:
            float _fov;
            float _aspect;
        };

        class OrthographicCamera : public Camera
        {
        public:
            OrthographicCamera(const std::string &name = "orthographic_camera");
            OrthographicCamera(const OrthographicCamera &other);
            virtual ~OrthographicCamera();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            virtual const glm::mat4 getProjectionMatrix() const override;

            glm::vec2 getSize() const;
            void setSize(const glm::vec2 &size);

        protected:
            glm::vec2 _size;
        };

    } // namespace Scene

} // namespace Stone
