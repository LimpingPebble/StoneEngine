// Copyright 2024 Stone-Engine

#include "Node3D.hpp"

namespace STN
{

    namespace Scene
    {
        
        Node3D::Node3D(const std::string &name)
            : Node(name), _transform()
        {
        }

        Node3D::Node3D(const Node3D &other)
            : Node(other), _transform(other._transform)
        {
        }

        Node3D::~Node3D()
        {
        }

        const char *Node3D::getClassName() const
        {
            return "Node3D";
        }

        Transform3D &Node3D::getTransform()
        {
            return _transform;
        }

        const Transform3D &Node3D::getTransform() const
        {
            return _transform;
        }

        void Node3D::setTransform(const Transform3D &transform)
        {
            _transform = transform;
        }

        const glm::mat4& Node3D::getTransformMatrix()
        {
            return _transform.getTransformMatrix();
        }

        const glm::mat4 Node3D::getTransformMatrix() const
        {
            return _transform.getTransformMatrix();
        }

        void Node3D::render(RenderContext& context)
        {
            glm::mat4 previousModelMatrix = context.modelMatrix;

            context.modelMatrix = context.modelMatrix * getTransformMatrix();
            for (auto child : getChildren())
            {
                child->render(context);
            }
            context.modelMatrix = previousModelMatrix;
        }

        std::string Node3D::debugDescription(bool colored) const
        {
            (void)colored;
            std::string description;
            bool needComma = false;
            const glm::vec3& pos = _transform.getPosition();
            if (pos != glm::vec3(0.0f))
            {
                description += "position:{x:" + std::to_string(pos.x) + ",y:" + std::to_string(pos.y) + ",z:" + std::to_string(pos.z) + "}";
                needComma = true;
            }
            glm::vec3 rot = _transform.getEulerAngles();
            if (rot != glm::vec3(0.0f))
            {
                if (needComma)
                    description += ",";
                description += "rotation:{rx:" + std::to_string(rot.x) + ",ry:" + std::to_string(rot.y) + ",rz:" + std::to_string(rot.z) + "}";
            }
            const glm::vec3& scale = _transform.getScale();
            if (scale != glm::vec3(1.0f))
            {
                if (needComma)
                    description += ",";
                description += "scale:{x:" + std::to_string(scale.x) + ",y:" + std::to_string(scale.y) + ",z:" + std::to_string(scale.z) + "}";
            }
            return "{" + description + "}";
        }

        const char *Node3D::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_CYAN;
        }

    } // namespace Scene

} // namespace STN