// Copyright 2024 Stone-Engine

#include "Window/GlfwWindow.hpp"
#include <stdexcept>
#include <iostream>

namespace Stone::Window
{

    GlfwWindow::GlfwWindow(std::shared_ptr<App> app, const WindowSettings &settings)
        : Window(app, settings),
          _glfwWindow(nullptr),
          _mousePosition(0.0f, 0.0f)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);

        GLFWwindow *sharingContext = NULL;
        if (!settings.shareContext.expired())
        {
            std::shared_ptr<GlfwWindow> sharingGlfwWindow = std::dynamic_pointer_cast<GlfwWindow>(settings.shareContext.lock());
            if (sharingGlfwWindow)
            {
                sharingContext = sharingGlfwWindow->_glfwWindow;
            }
        }

        _glfwWindow = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(),
                                       settings.fullScreen ? glfwGetPrimaryMonitor() : NULL,
                                       sharingContext);
        if (!_glfwWindow)
        {
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(_glfwWindow);

        _initializeWindowCallbacks();

        glfwSwapInterval(1);

        _elapsedTime = glfwGetTime();
        _deltaTime = 0;
    }

    GlfwWindow::~GlfwWindow()
    {
        if (_glfwWindow != NULL)
        {
            glfwDestroyWindow(_glfwWindow);
        }
    }

    void GlfwWindow::loopOnce()
    {
        if (_glfwWindow == NULL)
            return;

        glfwPollEvents();

        if (glfwWindowShouldClose(_glfwWindow))
            return;

        double newTime = glfwGetTime();
        _deltaTime = newTime - _elapsedTime;
        _elapsedTime = newTime;

        // TODO: Update the scene and render it

        glfwSwapBuffers(_glfwWindow);
    }

    bool GlfwWindow::shouldClose() const
    {
        return _glfwWindow == NULL || glfwWindowShouldClose(_glfwWindow);
    }

    void GlfwWindow::_initializeWindowCallbacks()
    {
        glfwSetWindowUserPointer(_glfwWindow, this);

        glfwSetCursorPosCallback(_glfwWindow, [](GLFWwindow *win, double x, double y)
                                 {
            GlfwWindow *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
            window->_onMouseMoveCallback(x, y); });

        glfwSetMouseButtonCallback(_glfwWindow, [](GLFWwindow *win, int button, int action, int modifiers)
                                   {
            GlfwWindow *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
            window->_onMouseButtonCallback(button, action, modifiers); });

        glfwSetKeyCallback(_glfwWindow, [](GLFWwindow *win, int key, int scancode, int action, int mods)
                           {
            GlfwWindow *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		    window->_onKeyCallback(key, scancode, action, mods); });

        glfwSetCharCallback(_glfwWindow, [](GLFWwindow *win, unsigned int codepoint)
                            {
            GlfwWindow *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
            window->_onCharCallback(codepoint); });

        glfwSetWindowCloseCallback(_glfwWindow, [](GLFWwindow *win)
                                   {
            GlfwWindow *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
            window->_onCloseCallback(); });

        glfwSetWindowSizeCallback(_glfwWindow, [](GLFWwindow *win, int width, int height)
                                  {
            GlfwWindow *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
            if (window->_glfwWindow == NULL)
                return;
            glfwGetWindowSize(window->_glfwWindow, &(window->_settings.width), &(window->_settings.height));
            window->_onResizeCallback(width, height); });

        glfwSetScrollCallback(_glfwWindow, [](GLFWwindow *win, double x, double y)
                              {
            GlfwWindow *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
            window->_onScrollCallback(x, y); });
    }

} // namespace Stone::Window
