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

        std::string Scene::debugDescription() const
        {
            std::string str = Node::debugDescription();
            str.pop_back();
            str += ",camera:";
            str += (_camera.expired() ? "nullptr" : _camera.lock()->getGlobalName());
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

            context.stage = RenderStage::PreProcessing;
            Node::render(context);

            context.stage = RenderStage::Opaque;
            Node::render(context);

            context.stage = RenderStage::Transparent;
            Node::render(context);

            context.stage = RenderStage::CustomStencil;
            Node::render(context);

            context.stage = RenderStage::PostProcessing;
            Node::render(context);
        }

        const char *Scene::_termClassColor() const
        {
            return TERM_COLOR_RED;
        }

    } // namespace Scene

} // namespace STN
