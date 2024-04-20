// Copyright 2024 Stone-Engine

#include "scene/Material.hpp"
#include "scene/ISceneRenderer.hpp"

namespace Stone
{

    namespace Scene
    {

        Material::Material()
            : Object(), IRenderElement()
        {
        }

        Material::Material(const Material &other)
            : Object(other), IRenderElement(other)
        {
        }

        Material::~Material()
        {
        }

        const char *Material::getClassName() const
        {
            return "Material";
        }

        std::string Material::debugDescription() const
        {
            std::string str = Object::debugDescription();
            str.pop_back();
            str += "}";
            return str;
        }

        void Material::generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer)
        {
            renderer->generateDataForMaterial(std::static_pointer_cast<Material>(shared_from_this()));
        }

    } // namespace Scene

} // namespace Stone