// Copyright 2024 Stone-Engine

#include "Window/App.hpp"
#include "Window/Window.hpp"
// #include "Window/GlfwWindow.hpp"
#include <algorithm>


namespace Stone::Window
{

    App::App() : std::enable_shared_from_this<App>(), _windows()
    {
    }

    App::~App()
    {
    }

    std::shared_ptr<Window> App::createWindow(const WindowSettings &settings)
    {
        std::shared_ptr<Window> win = std::make_shared<Window>(shared_from_this(), settings);
        _windows.push_back(win);
        return win;
    }

    void App::destroyWindow(std::shared_ptr<Window> window)
    {
        auto it = std::find(_windows.begin(), _windows.end(), window);
        if (it != _windows.end())
        {
            _windows.erase(it);
        }
    }

    void App::run()
    {
        while (true)
        {
            for (auto &window : _windows)
            {
                window->loopOnce();
            }
        }
    }

} // namespace Stone::Window
