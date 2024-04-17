// Copyright 2024 Stone-Engine

#include "Transform.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace STN
{

    Transform2D::Transform2D()
        : _position(0.0f, 0.0f), _rotation(0.0f), _scale(1.0f, 1.0f), _transformMatrix(1.0f), _transformMatrixDirty(true)
    {
    }

    Transform2D::Transform2D(const Transform2D &other)
        : _position(other._position), _rotation(other._rotation), _scale(other._scale), _transformMatrix(other._transformMatrix), _transformMatrixDirty(other._transformMatrixDirty)
    {
    }

    Transform2D::~Transform2D()
    {
    }

    void Transform2D::setPosition(const glm::vec2 &position)
    {
        _position = position;
        _transformMatrixDirty = true;
    }

    void Transform2D::setRotation(float rotation)
    {
        _rotation = rotation;
        _transformMatrixDirty = true;
    }

    void Transform2D::setScale(const glm::vec2 &scale)
    {
        _scale = scale;
        _transformMatrixDirty = true;
    }

    const glm::vec2 &Transform2D::getPosition() const
    {
        return _position;
    }

    float Transform2D::getRotation() const
    {
        return _rotation;
    }

    const glm::vec2 &Transform2D::getScale() const
    {
        return _scale;
    }

    void Transform2D::translate(const glm::vec2 &translation)
    {
        _position += translation;
        _transformMatrixDirty = true;
    }

    void Transform2D::rotate(float rotation)
    {
        _rotation += rotation;
        _transformMatrixDirty = true;
    }

    void Transform2D::scale(const glm::vec2 &scale)
    {
        _scale *= scale;
        _transformMatrixDirty = true;
    }

    const glm::mat3 &Transform2D::getTransformMatrix()
    {
        if (_transformMatrixDirty)
        {
            calculateTransformMatrix(_transformMatrix);
            _transformMatrixDirty = false;
        }
        return _transformMatrix;
    }

    glm::mat3 Transform2D::getTransformMatrix() const
    {
        if (!_transformMatrixDirty)
        {
            return _transformMatrix;
        }

        glm::mat3 transformMatrix;
        calculateTransformMatrix(transformMatrix);
        return transformMatrix;
    }

    static float &mat3ValueAt(glm::mat3 &m, int l, int c)
    {
        return glm::value_ptr(m)[3 * c + l];
    }

    // TODO: Implement this better
    void Transform2D::calculateTransformMatrix(glm::mat3 &m) const
    {
        m = glm::mat3(1.0f);
        float cosa = cos(_rotation);
        float sina = sin(_rotation);
        mat3ValueAt(m, 0, 0) = _scale.x * cosa;
        mat3ValueAt(m, 1, 0) = _scale.x * sina;
        mat3ValueAt(m, 2, 0) = 0;
        mat3ValueAt(m, 0, 1) = _scale.y * -sina;
        mat3ValueAt(m, 1, 1) = _scale.y * cosa;
        mat3ValueAt(m, 2, 1) = 0;
        mat3ValueAt(m, 0, 2) = _position.x;
        mat3ValueAt(m, 1, 2) = _position.y;
        mat3ValueAt(m, 2, 2) = 1;
    }

} // namespace STN
