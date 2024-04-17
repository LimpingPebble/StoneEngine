// Copyright 2024 Stone-Engine

#include "Scene.hpp"

namespace STN
{

    namespace Scene
    {

        Scene::Scene(const std::string &name)
            : Node(name)
        {
        }

        Scene::Scene(const Scene &other)
            : Node(other)
        {
            _camera = other._camera;
        }

        Scene::~Scene()
        {
        }

        const char *Scene::getClassName() const
        {
            return "Scene";
        }

        std::string Scene::debugDescription(bool colored) const
        {
            (void)colored;
            std::string str;
            str += "{camera:";
            if (_camera.expired())
            {
                str += "none";
            }
            else
            {
                str += _camera.lock()->getGlobalName();
            }
            str += "}";
            return str;
        }

        void Scene::setCamera(std::shared_ptr<Camera> camera)
        {
            _camera = camera;
        }

        std::shared_ptr<Camera> Scene::getCamera() const
        {
            return _camera.lock();
        }

        void Scene::render()
        {
            RenderContext context;

            if (auto camera = _camera.lock())
            {
                context.viewMatrix = glm::inverse(camera->getWorldTransformMatrix());
                context.projectionMatrix = camera->getProjectionMatrix();
            }

            std::shared_ptr<Scene> scene = std::dynamic_pointer_cast<Scene>(shared_from_this());

            Node::render(context, RenderStage::PreProcessing, scene);
            Node::render(context, RenderStage::Opaque, scene);
            Node::render(context, RenderStage::Transparent, scene);
            Node::render(context, RenderStage::CustomStencil, scene);
            Node::render(context, RenderStage::PostProcessing, scene);
        }

        const char *Scene::_termClassColor() const
        {
            return TERM_COLOR_RED;
        }

    } // namespace Scene

} // namespace STN
