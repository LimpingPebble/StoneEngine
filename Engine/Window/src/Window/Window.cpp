// Copyright 2024 Stone-Engine

#pragma once

#include "Window/Window.hpp"
#include <iostream>

namespace Stone::Window
{

    Window::Window(std::shared_ptr<App> app, const WindowSettings &settings) :
        _app(app), _settings(settings)
    {
        std::cout << "new window of size " << settings.width << "x" << settings.height << " with title " << settings.title << std::endl;
    }

    Window::~Window()
    {
        std::cout << "window destroyed" << std::endl;
    }

    void Window::loopOnce()
    {
        std::cout << "window loop" << std::endl;
    }

    const WindowSettings &Window::getSettings() const
    {
        return _settings;
    }

    std::shared_ptr<App> Window::getApp() const
    {
        return _app.lock();
    }

    void Window::setWorld(std::shared_ptr<Stone::Scene::World> world)
    {
        _world = world;
    }

    std::shared_ptr<Stone::Scene::World> Window::getWorld() const
    {
        return _world;
    }

}