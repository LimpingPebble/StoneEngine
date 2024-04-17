// Copyright 2024 Stone-Engine

#pragma once

#include "Object.hpp"
#include <glm/glm.hpp>
#include <string>

namespace STN
{

    class Texture : public Object
    {
    public:
        Texture();
        Texture(const Texture &other);

        virtual ~Texture();

        void setSize(const glm::ivec2 &size);
        glm::ivec2 getSize() const;

        void setFilePath(const std::string &texture);
        const std::string &getFilePath() const;

    protected:
        glm::ivec2 _size;
        std::string _filePath;
    };

} // namespace STN
