// Copyright 2024 Stone-Engine

#include "Skybox.hpp"
#include "Texture.hpp"

namespace STN
{

    namespace Scene
    {

        Skybox::Skybox(const std::string &name) : Node(name), _textures()
        {
        }

        Skybox::Skybox(const Skybox &other) : Node(other), _textures(other._textures)
        {
        }

        Skybox::~Skybox()
        {
        }

        const char *Skybox::getClassName() const
        {
            return "Skybox";
        }

        std::string Skybox::debugDescription() const
        {
            std::string str = Node::debugDescription();
            str.pop_back();
            str += ",textures:[";
            for (size_t i = 0; i < 6; i++)
            {
                str += (_textures[i] ? std::to_string(_textures[i]->getId()) : "none") + std::string(",");
            }
            str.pop_back();
            str += "]}";
            return str;
        }

        void Skybox::setTexture(const std::shared_ptr<Texture> &texture, size_t index)
        {
            assert(index < 6);
            _textures[index] = texture;
        }

        std::shared_ptr<Texture> Skybox::getTexture(size_t index) const
        {
            assert(index < 6);
            return _textures[index];
        }

        const char *Skybox::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_GREEN;
        }

    } // namespace Scene

} // namespace STN
