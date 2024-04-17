// Copyright 2024 Stone-Engine

#include "Camera.hpp"
#include <glm/ext.hpp>

namespace STN
{

    namespace Scene
    {

        Camera::Camera(const std::string &name)
            : Node(name)
        {
        }

        Camera::Camera(const Camera &other)
            : Node(other)
        {
        }

        Camera::~Camera()
        {
        }

        const char *Camera::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_BLUE;
        }

        PerspectiveCamera::PerspectiveCamera(const std::string &name)
            : Camera(name), _fov(glm::radians(45.0f)), _aspect(1.0f), _near(0.1f), _far(100.0f)
        {
        }

        PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera &other)
            : Camera(other)
        {
            _fov = other._fov;
            _aspect = other._aspect;
            _near = other._near;
            _far = other._far;
        }

        PerspectiveCamera::~PerspectiveCamera()
        {
        }

        const char *PerspectiveCamera::getClassName() const
        {
            return "PerspectiveCamera";
        }

        std::string PerspectiveCamera::debugDescription(bool colored) const
        {
            (void)colored;
            std::string str = "{";
            str += "fov:" + std::to_string(_fov) + ",";
            str += "aspect:" + std::to_string(_aspect) + ",";
            str += "near:" + std::to_string(_near) + ",";
            str += "far:" + std::to_string(_far) + "}";
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

        float PerspectiveCamera::getNear() const
        {
            return _near;
        }

        void PerspectiveCamera::setNear(float near)
        {
            _near = near;
        }

        float PerspectiveCamera::getFar() const
        {
            return _far;
        }

        void PerspectiveCamera::setFar(float far)
        {
            _far = far;
        }

        OrthographicCamera::OrthographicCamera(const std::string &name)
            : Camera(name), _left(-1.0f), _right(1.0f), _bottom(-1.0f), _top(1.0f), _near(0.1f), _far(100.0f)
        {
        }

        OrthographicCamera::OrthographicCamera(const OrthographicCamera &other)
            : Camera(other)
        {
            _left = other._left;
            _right = other._right;
            _bottom = other._bottom;
            _top = other._top;
            _near = other._near;
            _far = other._far;
        }

        OrthographicCamera::~OrthographicCamera()
        {
        }

        const char *OrthographicCamera::getClassName() const
        {
            return "OrthographicCamera";
        }

        std::string OrthographicCamera::debugDescription(bool colored) const
        {
            (void)colored;
            std::string str = "{";
            str += "left:" + std::to_string(_left) + ",";
            str += "right:" + std::to_string(_right) + ",";
            str += "bottom:" + std::to_string(_bottom) + ",";
            str += "top:" + std::to_string(_top) + ",";
            str += "near:" + std::to_string(_near) + ",";
            str += "far:" + std::to_string(_far) + "}";
            return str;
        }

        const glm::mat4 OrthographicCamera::getProjectionMatrix() const
        {
            return glm::ortho(_left, _right, _bottom, _top, _near, _far);
        }

        float OrthographicCamera::getLeft() const
        {
            return _left;
        }

        void OrthographicCamera::setLeft(float left)
        {
            _left = left;
        }

        float OrthographicCamera::getRight() const
        {
            return _right;
        }

        void OrthographicCamera::setRight(float right)
        {
            _right = right;
        }

        float OrthographicCamera::getBottom() const
        {
            return _bottom;
        }

        void OrthographicCamera::setBottom(float bottom)
        {
            _bottom = bottom;
        }

        float OrthographicCamera::getTop() const
        {
            return _top;
        }

        void OrthographicCamera::setTop(float top)
        {
            _top = top;
        }

        float OrthographicCamera::getNear() const
        {
            return _near;
        }

        void OrthographicCamera::setNear(float near)
        {
            _near = near;
        }

        float OrthographicCamera::getFar() const
        {
            return _far;
        }

        void OrthographicCamera::setFar(float far)
        {
            _far = far;
        }

    } // namespace Scene

} // namespace STN
