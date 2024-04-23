// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene
{

    class CameraNode : public PivotNode
    {
    public:
        STONE_ABSTRACT_NODE(CameraNode);

        CameraNode(const std::string &name = "camera");
        CameraNode(const CameraNode &other);

        virtual ~CameraNode();

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

    class PerspectiveCameraNode : public CameraNode
    {
    public:
        STONE_NODE(PerspectiveCameraNode);

        PerspectiveCameraNode(const std::string &name = "perspective_camera");
        PerspectiveCameraNode(const PerspectiveCameraNode &other);

        virtual ~PerspectiveCameraNode();

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

    class OrthographicCameraNode : public CameraNode
    {
    public:
        STONE_NODE(OrthographicCameraNode);

        OrthographicCameraNode(const std::string &name = "orthographic_camera");
        OrthographicCameraNode(const OrthographicCameraNode &other);
        virtual ~OrthographicCameraNode();

        virtual std::string debugDescription() const override;

        virtual const glm::mat4 getProjectionMatrix() const override;

        glm::vec2 getSize() const;
        void setSize(const glm::vec2 &size);

    protected:
        glm::vec2 _size;
    };

} // namespace Stone::Scene
