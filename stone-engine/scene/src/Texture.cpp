// Copyright 2024 Stone-Engine

#include "Texture.hpp"

namespace STN
{

    Texture::Texture()
        : Object(), _size(0, 0), _filePath()
    {
    }

    Texture::Texture(const Texture &other)
        : Object(other), _size(other._size), _filePath(other._filePath)
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

} // namespace STN