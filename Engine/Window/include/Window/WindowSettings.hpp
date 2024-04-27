// Copyright 2024 Stone-Engine

#pragma once

#include <string>
#include <memory>

namespace Stone::Window
{

    struct WindowSettings
    {
        int width = 800;
        int height = 600;
        std::string title = "Stone Engine";
        bool fullScreen = false;
        bool resizable = true;
        std::weak_ptr<class Window> shareContext;
    };

} // namespace Stone::Window
