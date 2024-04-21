// Copyright 2024 Stone-Engine

#include "scene/Skybox.hpp"
#include "scene/Texture.hpp"
#include "scene/ISceneRenderer.hpp"

namespace Stone
{

    namespace Scene
    {

        STONE_NODE_IMPLEMENTATION(Skybox)

        Skybox::Skybox(const std::string &name)
            : RenderableNode(name), _textures()
        {
        }

        Skybox::Skybox(const Skybox &other)
            : RenderableNode(other), _textures(other._textures)
        {
        }

        Skybox::~Skybox()
        {
        }

        std::string Skybox::debugDescription() const
        {
            std::string str = RenderableNode::debugDescription();
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

        void Skybox::generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer)
        {
            renderer->generateDataForSkybox(std::static_pointer_cast<Skybox>(shared_from_this()));
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

} // namespace Stone
