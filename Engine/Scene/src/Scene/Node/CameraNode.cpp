// Copyright 2024 Stone-Engine

#include "Scene/Node/CameraNode.hpp"
#include <glm/ext.hpp>
#include "GlmUtilities.hpp"

namespace Stone::Scene
{

    STONE_ABSTRACT_NODE_IMPLEMENTATION(CameraNode)

    CameraNode::CameraNode(const std::string &name)
        : PivotNode(name), _near(0.1f), _far(100.0f)
    {
    }

    CameraNode::CameraNode(const CameraNode &other)
        : PivotNode(other), _near(other._near), _far(other._far)
    {
    }

    CameraNode::~CameraNode()
    {
    }

    std::string CameraNode::debugDescription() const
    {
        std::string str = PivotNode::debugDescription();
        str.pop_back();
        str += ",near:" + std::to_string(_near);
        str += ",far:" + std::to_string(_far) + "}";
        return str;
    }

    float CameraNode::getNear() const
    {
        return _near;
    }

    void CameraNode::setNear(float near)
    {
        _near = near;
    }

    float CameraNode::getFar() const
    {
        return _far;
    }

    void CameraNode::setFar(float far)
    {
        _far = far;
    }

    const char *CameraNode::_termClassColor() const
    {
        return TERM_COLOR_BOLD TERM_COLOR_BLUE;
    }

    STONE_NODE_IMPLEMENTATION(PerspectiveCameraNode)

    PerspectiveCameraNode::PerspectiveCameraNode(const std::string &name)
        : CameraNode(name), _fov(glm::radians(45.0f)), _aspect(1.0f)
    {
    }

    PerspectiveCameraNode::PerspectiveCameraNode(const PerspectiveCameraNode &other)
        : CameraNode(other), _fov(other._fov), _aspect(other._aspect)
    {
    }

    PerspectiveCameraNode::~PerspectiveCameraNode()
    {
    }

    std::string PerspectiveCameraNode::debugDescription() const
    {
        std::string str = CameraNode::debugDescription();
        str.pop_back();
        str += ",fov:" + std::to_string(_fov);
        str += ",aspect:" + std::to_string(_aspect) + "}";
        return str;
    }

    const glm::mat4 PerspectiveCameraNode::getProjectionMatrix() const
    {
        return glm::perspective(_fov, _aspect, _near, _far);
    }

    float PerspectiveCameraNode::getFov() const
    {
        return _fov;
    }

    void PerspectiveCameraNode::setFov(float fov)
    {
        _fov = fov;
    }

    float PerspectiveCameraNode::getAspect() const
    {
        return _aspect;
    }

    void PerspectiveCameraNode::setAspect(float aspect)
    {
        _aspect = aspect;
    }

    STONE_NODE_IMPLEMENTATION(OrthographicCameraNode)

    OrthographicCameraNode::OrthographicCameraNode(const std::string &name)
        : CameraNode(name), _size(10.0f)
    {
    }

    OrthographicCameraNode::OrthographicCameraNode(const OrthographicCameraNode &other)
        : CameraNode(other), _size(other._size)
    {
    }

    OrthographicCameraNode::~OrthographicCameraNode()
    {
    }

    std::string OrthographicCameraNode::debugDescription() const
    {
        std::string str = CameraNode::debugDescription();
        str.pop_back();
        str += ",size:" + std::to_string(_size) + "}";
        return str;
    }

    const glm::mat4 OrthographicCameraNode::getProjectionMatrix() const
    {
        return glm::ortho(-_size.x / 2, _size.x / 2, -_size.y / 2, _size.y / 2, _near, _far);
    }

} // namespace Stone::Scene
