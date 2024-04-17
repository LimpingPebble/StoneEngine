// Copyright 2024 Stone-Engine

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace STN
{

    namespace Scene
    {

        class Transform2D
        {
        public:
            Transform2D();
            Transform2D(const Transform2D &other);

            virtual ~Transform2D();

            void setPosition(const glm::vec2 &position);
            void setRotation(float rotation);
            void setScale(const glm::vec2 &scale);

            const glm::vec2 &getPosition() const;
            float getRotation() const;
            const glm::vec2 &getScale() const;

            void translate(const glm::vec2 &translation);
            void rotate(float rotation);
            void scale(const glm::vec2 &scale);

            /** Update if needed and return the transform matrix */
            const glm::mat3 &getTransformMatrix();
            /** Return the transform matrix if cached or calculate it */
            glm::mat3 getTransformMatrix() const;

        protected:
            glm::vec2 _position;
            float _rotation;
            glm::vec2 _scale;

            glm::mat3 _transformMatrix;
            bool _transformMatrixDirty;
        };

        class Transform3D
        {
        public:
            Transform3D();
            Transform3D(const Transform3D &other);

            virtual ~Transform3D();

            void setPosition(const glm::vec3 &position);
            void setRotation(const glm::vec3 &rotation);
            void setScale(const glm::vec3 &scale);

            const glm::vec3 &getPosition() const;
            const glm::vec3 &getRotation() const;
            const glm::vec3 &getScale() const;

            void translate(const glm::vec3 &translation);
            void rotate(const glm::vec3 &rotation);
            void scale(const glm::vec3 &scale);

            /** Update if needed and return the transform matrix */
            const glm::mat4 &getTransformMatrix();
            /** Return the transform matrix if cached or calculate it */
            glm::mat4 getTransformMatrix() const;

        protected:
            glm::vec3 _position;
            glm::vec3 _rotation;
            glm::vec3 _scale;

            glm::mat4 _transformMatrix;
            bool _transformMatrixDirty;
        };

    } // namespace Scene

} // namespace STN
