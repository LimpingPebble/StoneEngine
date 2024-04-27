// Copyright 2024 Stone-Engine

#pragma once

#include "Window/Window.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Stone::Window
{

    class GlfwWindow : public Window
    {
    public:
        GlfwWindow() = delete;
        GlfwWindow(std::shared_ptr<App> app, const WindowSettings &settings);

        virtual ~GlfwWindow();

        virtual void loopOnce() override;

        virtual bool shouldClose() const override;

        /** Window events */

    private:
        GLFWwindow *_glfwWindow;
        std::pair<float, float> _mousePosition;

        float _elapsedTime;
        float _deltaTime;

        void _initializeWindowCallbacks();
    };

} // namespace Stone::Window
