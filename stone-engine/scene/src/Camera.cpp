// Copyright 2024 Stone-Engine

#include "scene/Camera.hpp"
#include <glm/ext.hpp>

namespace Stone
{

    namespace Scene
    {

        Camera::Camera(const std::string &name)
            : Node(name), _near(0.1f), _far(100.0f)
        {
        }

        Camera::Camera(const Camera &other)
            : Node(other), _near(other._near), _far(other._far)
        {
        }

        Camera::~Camera()
        {
        }

        const char *Camera::getClassName() const
        {
            return "Camera";
        }

        std::string Camera::debugDescription() const
        {
            std::string str = Node::debugDescription();
            str.pop_back();
            str += ",near:" + std::to_string(_near);
            str += ",far:" + std::to_string(_far) + "}";
            return str;
        }

        float Camera::getNear() const
        {
            return _near;
        }

        void Camera::setNear(float near)
        {
            _near = near;
        }

        float Camera::getFar() const
        {
            return _far;
        }

        void Camera::setFar(float far)
        {
            _far = far;
        }

        const char *Camera::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_BLUE;
        }

        PerspectiveCamera::PerspectiveCamera(const std::string &name)
            : Camera(name), _fov(glm::radians(45.0f)), _aspect(1.0f)
        {
        }

        PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera &other)
            : Camera(other), _fov(other._fov), _aspect(other._aspect)
        {
        }

        PerspectiveCamera::~PerspectiveCamera()
        {
        }

        const char *PerspectiveCamera::getClassName() const
        {
            return "PerspectiveCamera";
        }

        std::string PerspectiveCamera::debugDescription() const
        {
            std::string str = Camera::debugDescription();
            str.pop_back();
            str += ",fov:" + std::to_string(_fov);
            str += ",aspect:" + std::to_string(_aspect) + "}";
            return str;
        }

        const glm::mat4 PerspectiveCamera::getProjectionMatrix() const
        {
            return glm::perspective(_fov, _aspect, _near, _far);
        }

        float PerspectiveCamera::getFov() const
        {
            return _fov;
        }

        void PerspectiveCamera::setFov(float fov)
        {
            _fov = fov;
        }

        float PerspectiveCamera::getAspect() const
        {
            return _aspect;
        }

        void PerspectiveCamera::setAspect(float aspect)
        {
            _aspect = aspect;
        }

        OrthographicCamera::OrthographicCamera(const std::string &name)
            : Camera(name), _size(10.0f)
        {
        }

        OrthographicCamera::OrthographicCamera(const OrthographicCamera &other)
            : Camera(other), _size(other._size)
        {
        }

        OrthographicCamera::~OrthographicCamera()
        {
        }

        const char *OrthographicCamera::getClassName() const
        {
            return "OrthographicCamera";
        }

        std::string OrthographicCamera::debugDescription() const
        {
            std::string str = Camera::debugDescription();
            str.pop_back();
            str += ",size:[" + std::to_string(_size.x) + "," + std::to_string(_size.y) + "]}";
            return str;
        }

        const glm::mat4 OrthographicCamera::getProjectionMatrix() const
        {
            return glm::ortho(-_size.x / 2, _size.x / 2, -_size.y / 2, _size.y / 2, _near, _far);
        }

    } // namespace Scene

} // namespace Stone
