// Copyright 2024 Stone-Engine

#pragma once

#include "Window/WindowSettings.hpp"
#include <memory>

namespace Stone::Scene
{
    class World;
}

namespace Stone::Window
{

    class App;

    class Window
    {
    public:
        Window() = delete;
        Window(std::shared_ptr<App> app, const WindowSettings &settings);

        virtual ~Window();

        virtual void loopOnce();

        const WindowSettings &getSettings() const;
        std::shared_ptr<App> getApp() const;

        virtual void setWorld(std::shared_ptr<Stone::Scene::World> world);
        std::shared_ptr<Stone::Scene::World> getWorld() const;

    protected:
        std::weak_ptr<App> _app;
        WindowSettings _settings;
        std::shared_ptr<Stone::Scene::World> _world;
    };

} // namespace Stone::Window
