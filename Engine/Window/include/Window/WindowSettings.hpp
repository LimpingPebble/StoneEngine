// Copyright 2024 Stone-Engine

#pragma once

#include <string>

namespace Stone::Window
{

    struct WindowSettings
    {
        int width = 800;
        int height = 600;
        std::string title = "Stone Engine";
        bool fullScreen = false;
    };

} // namespace Stone::Window
