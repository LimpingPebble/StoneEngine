// Copyright 2024 Stone-Engine

#pragma once

#include "Node3D.hpp"

namespace STN
{

    namespace Scene
    {

        class Camera : public Node3D
        {
        public:
            Camera();
            Camera(const Camera &other);

            virtual ~Camera();

            virtual void updateProjectionMatrix() = 0;
            const glm::mat4 &getProjectionMatrix() const;

        protected:
            glm::mat4 _projectionMatrix;
        };

        class PerspectiveCamera : public Camera
        {
        public:
            PerspectiveCamera();
            PerspectiveCamera(const PerspectiveCamera &other);
            virtual ~PerspectiveCamera();

            virtual void updateProjectionMatrix() override;

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
            OrthographicCamera();
            OrthographicCamera(const OrthographicCamera &other);
            virtual ~OrthographicCamera();

            virtual void updateProjectionMatrix() override;

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
