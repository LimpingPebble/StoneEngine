// Copyright 2024 Stone-Engine

#include "Scene/RenderElement/Material.hpp"
#include "Scene/ISceneRenderer.hpp"
#include "Scene/RenderElement/Texture.hpp"
#include "GlmUtilities.hpp"

namespace Stone::Scene
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
        str += ",textures:{";
        for (auto &it : _textures)
        {
            str += it.first + ":" + (it.second ? std::to_string(it.second->getId()) : "null") + ",";
        }
        str.pop_back();
        str += "},vectors:{";
        for (auto &it : _vectors)
        {
            str += it.first + ":" + std::to_string(it.second) + ",";
        }
        str.pop_back();
        str += "},scalars:{";
        for (auto &it : _scalars)
        {
            str += it.first + ":" + std::to_string(it.second) + ",";
        }
        str.pop_back();
        str += "}";
        return str;
    }

    void Material::generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer)
    {
        renderer->generateDataForMaterial(std::static_pointer_cast<Material>(shared_from_this()));
    }

    void Material::setTextureParameter(const std::string &name, std::shared_ptr<Texture> texture)
    {
        _textures[name] = texture;
        markDirty();
    }

    std::shared_ptr<Texture> Material::getTextureParameter(const std::string &name) const
    {
        auto it = _textures.find(name);
        if (it != _textures.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void Material::setVectorParameter(const std::string &name, const glm::vec3 &vector)
    {
        _vectors[name] = vector;
        markDirty();
    }

    glm::vec3 Material::getVectorParameter(const std::string &name) const
    {
        auto it = _vectors.find(name);
        if (it != _vectors.end())
        {
            return it->second;
        }
        return glm::vec3(0.0f);
    }

    void Material::setScalarParameter(const std::string &name, float scalar)
    {
        _scalars[name] = scalar;
        markDirty();
    }

    float Material::getScalarParameter(const std::string &name) const
    {
        auto it = _scalars.find(name);
        if (it != _scalars.end())
        {
            return it->second;
        }
        return 0.0f;
    }

} // namespace Stone::Scene
