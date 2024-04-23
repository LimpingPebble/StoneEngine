// Copyright 2024 Stone-Engine

#include "Scene/RenderElement/Texture.hpp"
#include "Scene/ISceneRenderer.hpp"
#include "Core/Image.hpp"

namespace Stone::Scene
{

    Texture::Texture()
        : Object(), IRenderElement(), _image(nullptr)
    {
    }

    Texture::Texture(const Texture &other)
        : Object(other), IRenderElement(other), _image(other._image)
    {
    }

    Texture::~Texture()
    {
    }

    const char *Texture::getClassName() const
    {
        return "Texture";
    }

    std::string Texture::debugDescription() const
    {
        std::string str = Object::debugDescription();
        str.pop_back();
        str += ",image:" + (_image ? std::to_string(_image->getId()) : "null") + "}";
        return str;
    }

    void Texture::generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer)
    {
        renderer->generateDataForTexture(std::static_pointer_cast<Texture>(shared_from_this()));
    }

    void Texture::setImage(std::shared_ptr<Core::Image> image)
    {
        _image = image;
        markDirty();
    }

    std::shared_ptr<Core::Image> Texture::getImage() const
    {
        return _image;
    }

} // namespace Stone::Scene
