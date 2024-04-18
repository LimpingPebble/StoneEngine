// Copyright 2024 Stone-Engine

#include "Pivot.hpp"

namespace STN
{

    namespace Scene
    {
        
        Pivot::Pivot(const std::string &name)
            : Node(name), _transform()
        {
        }

        Pivot::Pivot(const Pivot &other)
            : Node(other), _transform(other._transform)
        {
        }

        Pivot::~Pivot()
        {
        }

        const char *Pivot::getClassName() const
        {
            return "Pivot";
        }

        Transform3D &Pivot::getTransform()
        {
            return _transform;
        }

        const Transform3D &Pivot::getTransform() const
        {
            return _transform;
        }

        void Pivot::setTransform(const Transform3D &transform)
        {
            _transform = transform;
        }

        void Pivot::transformRelativeMatrix(glm::mat4 &relative) const
        {
            relative = getTransformMatrix() * relative;
        }

        const glm::mat4& Pivot::getTransformMatrix()
        {
            return _transform.getTransformMatrix();
        }

        const glm::mat4 Pivot::getTransformMatrix() const
        {
            return _transform.getTransformMatrix();
        }

        void Pivot::render(RenderContext& context)
        {
            glm::mat4 previousModelMatrix = context.modelMatrix;

            context.modelMatrix = context.modelMatrix * getTransformMatrix();
            for (auto child : getChildren())
            {
                child->render(context);
            }
            context.modelMatrix = previousModelMatrix;
        }

        std::string Pivot::debugDescription(bool colored) const
        {
            std::string str = Node::debugDescription(colored);
            str.pop_back();
            const glm::vec3& pos = _transform.getPosition();
            str += "position:[" + std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z) + "],";
            glm::vec3 rot = _transform.getEulerAngles();
            str += "rotation:[" + std::to_string(rot.x) + "," + std::to_string(rot.y) + "," + std::to_string(rot.z) + "],";
            const glm::vec3& scale = _transform.getScale();
            str += "scale:[" + std::to_string(scale.x) + "," + std::to_string(scale.y) + "," + std::to_string(scale.z) + "]}";
            return str;
        }

        const char *Pivot::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_RED;
        }

    } // namespace Scene

} // namespace STN
