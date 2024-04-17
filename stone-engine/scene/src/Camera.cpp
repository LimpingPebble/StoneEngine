// Copyright 2024 Stone-Engine

#include "Camera.hpp"
#include <glm/ext.hpp>

namespace STN
{

    namespace Scene
    {

        Camera::Camera(const std::string &name)
            : Node(name), _projectionMatrix(1.0f)
        {
        }

        Camera::Camera(const Camera &other)
            : Node(other)
        {
            _projectionMatrix = other._projectionMatrix;
        }

        Camera::~Camera()
        {
        }

        const glm::mat4 &Camera::getProjectionMatrix() const
        {
            return _projectionMatrix;
        }

        const char *Camera::_termClassColor() const
        {
            return "\033[1;36m";
        }

        PerspectiveCamera::PerspectiveCamera(const std::string &name)
            : Camera(name), _fov(45.0f), _aspect(1.0f), _near(0.1f), _far(100.0f)
        {
            updateProjectionMatrix();
        }

        PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera &other)
            : Camera(other)
        {
            _fov = other._fov;
            _aspect = other._aspect;
            _near = other._near;
            _far = other._far;
            updateProjectionMatrix();
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

        void PerspectiveCamera::updateProjectionMatrix()
        {
            _projectionMatrix = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
        }

        float PerspectiveCamera::getFov() const
        {
            return _fov;
        }

        void PerspectiveCamera::setFov(float fov)
        {
            _fov = fov;
            updateProjectionMatrix();
        }

        float PerspectiveCamera::getAspect() const
        {
            return _aspect;
        }

        void PerspectiveCamera::setAspect(float aspect)
        {
            _aspect = aspect;
            updateProjectionMatrix();
        }

        float PerspectiveCamera::getNear() const
        {
            return _near;
        }

        void PerspectiveCamera::setNear(float near)
        {
            _near = near;
            updateProjectionMatrix();
        }

        float PerspectiveCamera::getFar() const
        {
            return _far;
        }

        void PerspectiveCamera::setFar(float far)
        {
            _far = far;
            updateProjectionMatrix();
        }

        const char *PerspectiveCamera::_termClassColor() const
        {
            return "\033[1;35m";
        }

        OrthographicCamera::OrthographicCamera(const std::string &name)
            : Camera(name), _left(-1.0f), _right(1.0f), _bottom(-1.0f), _top(1.0f), _near(0.1f), _far(100.0f)
        {
            updateProjectionMatrix();
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
            updateProjectionMatrix();
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

        void OrthographicCamera::updateProjectionMatrix()
        {
            _projectionMatrix = glm::ortho(_left, _right, _bottom, _top, _near, _far);
        }

        float OrthographicCamera::getLeft() const
        {
            return _left;
        }

        void OrthographicCamera::setLeft(float left)
        {
            _left = left;
            updateProjectionMatrix();
        }

        float OrthographicCamera::getRight() const
        {
            return _right;
        }

        void OrthographicCamera::setRight(float right)
        {
            _right = right;
            updateProjectionMatrix();
        }

        float OrthographicCamera::getBottom() const
        {
            return _bottom;
        }

        void OrthographicCamera::setBottom(float bottom)
        {
            _bottom = bottom;
            updateProjectionMatrix();
        }

        float OrthographicCamera::getTop() const
        {
            return _top;
        }

        void OrthographicCamera::setTop(float top)
        {
            _top = top;
            updateProjectionMatrix();
        }

        float OrthographicCamera::getNear() const
        {
            return _near;
        }

        void OrthographicCamera::setNear(float near)
        {
            _near = near;
            updateProjectionMatrix();
        }

        float OrthographicCamera::getFar() const
        {
            return _far;
        }

        void OrthographicCamera::setFar(float far)
        {
            _far = far;
            updateProjectionMatrix();
        }

        const char *OrthographicCamera::_termClassColor() const
        {
            return "\033[1;34m";
        }

    } // namespace Scene

} // namespace STN
