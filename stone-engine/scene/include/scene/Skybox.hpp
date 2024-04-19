// Copyright 2024 Stone-Engine

#pragma once

#include "scene/Node.hpp"

namespace Stone
{

    namespace Scene
    {

        class Texture;

        class Skybox : public Node
        {
        public:
            Skybox(const std::string &name = "skybox");
            Skybox(const Skybox &other);

            virtual ~Skybox();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            void setTexture(const std::shared_ptr<Texture> &texture, size_t index);
            std::shared_ptr<Texture> getTexture(size_t index) const;

        protected:
            std::array<std::shared_ptr<Texture>, 6> _textures;

            virtual const char *_termClassColor() const override;
        };

    } // namespace Scene

} // namespace Stone
