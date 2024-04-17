// Copyright 2024 Stone-Engine

#pragma once

#include "Node.hpp"

namespace STN
{

    namespace Scene
    {

        class Camera : public Node
        {
        public:
            Camera(const std::string &name = "camera");
            Camera(const Camera &other);

            virtual ~Camera();

            virtual const glm::mat4 getProjectionMatrix() const = 0;

        protected:

            virtual const char *_termClassColor() const override;
        };

        class PerspectiveCamera : public Camera
        {
        public:
            PerspectiveCamera(const std::string &name = "perspective_camera");
            PerspectiveCamera(const PerspectiveCamera &other);
            virtual ~PerspectiveCamera();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription(bool colored) const override;

            virtual const glm::mat4 getProjectionMatrix() const override;

            float getFov() const;
            void setFov(float fov);

            float getAspect() const;
            void setAspect(float aspect);

            float getNear() const;
            void setNear(float near);

            float getFar() const;
            void setFar(float far);

        protected:
            float _fov;
            float _aspect;
            float _near;
            float _far;
        };

        class OrthographicCamera : public Camera
        {
        public:
            OrthographicCamera(const std::string &name = "orthographic_camera");
            OrthographicCamera(const OrthographicCamera &other);
            virtual ~OrthographicCamera();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription(bool colored) const override;

            virtual const glm::mat4 getProjectionMatrix() const override;

            float getLeft() const;
            void setLeft(float left);

            float getRight() const;
            void setRight(float right);

            float getBottom() const;
            void setBottom(float bottom);

            float getTop() const;
            void setTop(float top);

            float getNear() const;
            void setNear(float near);

            float getFar() const;
            void setFar(float far);

        protected:
            float _left;
            float _right;
            float _bottom;
            float _top;
            float _near;
            float _far;
        };

    } // namespace Scene

} // namespace STN
