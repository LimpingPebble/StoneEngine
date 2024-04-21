// Copyright 2024 Stone-Engine

#include "scene/Texture.hpp"
#include "scene/ISceneRenderer.hpp"

namespace Stone
{

    namespace Scene
    {

        Texture::Texture()
            : Object(), IRenderElement(), _size(0, 0), _filePath()
        {
        }

        Texture::Texture(const Texture &other)
            : Object(other), IRenderElement(other), _size(other._size), _filePath(other._filePath)
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
            str += ",size:[" + std::to_string(_size.x) + "," + std::to_string(_size.y) + "]";
            str += ",filePath:" + _filePath + "}";
            return str;
        }

        void Texture::generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer)
        {
            renderer->generateDataForTexture(std::static_pointer_cast<Texture>(shared_from_this()));
        }

        const glm::ivec2 &Texture::getSize() const
        {
            return _size;
        }

        void Texture::setFilePath(const std::string &texture)
        {
            _filePath = texture;
        }

        const std::string &Texture::getFilePath() const
        {
            return _filePath;
        }

    } // namespace Scene

} // namespace Stone