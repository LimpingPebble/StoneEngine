// Copyright 2024 Stone-Engine

#pragma once

#include "core/Object.hpp"
#include <glm/glm.hpp>
#include <string>

namespace Stone
{

    namespace Scene
    {

        class Texture : public Core::Object
        {
        public:
            Texture();
            Texture(const Texture &other);

            virtual ~Texture();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            const glm::ivec2 &getSize() const;

            void setFilePath(const std::string &texture);
            const std::string &getFilePath() const;

            // [...]

        protected:
            glm::ivec2 _size;
            std::string _filePath;
        };

    } // namespace Scene

} // namespace Stone
