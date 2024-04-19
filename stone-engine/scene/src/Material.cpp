// Copyright 2024 Stone-Engine

#include "scene/Material.hpp"

namespace Stone
{

    namespace Scene
    {

        Material::Material() : Object()
        {
        }

        Material::Material(const Material &other) : Object(other)
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

    } // namespace Scene

} // namespace Stone