// Copyright 2024 Stone-Engine

#include "Window/GlfwWindow.hpp"

#include "Render/VulkanRenderer.hpp"

#include <iostream>
#include <stdexcept>

namespace Stone::Window {

GlfwWindow::GlfwWindow(const std::shared_ptr<App> &app, const WindowSettings &settings)
	: Window(app, settings), _glfwWindow(nullptr) {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);

	GLFWwindow *sharingContext = nullptr;
	if (!settings.shareContext.expired()) {
		auto sharingGlfwWindow = std::dynamic_pointer_cast<GlfwWindow>(settings.shareContext.lock());
		if (sharingGlfwWindow) {
			sharingContext = sharingGlfwWindow->_glfwWindow;
			_renderer = sharingGlfwWindow->_renderer;
		}
	}

	_glfwWindow = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(),
								   settings.fullScreen ? glfwGetPrimaryMonitor() : nullptr, sharingContext);
	if (!_glfwWindow) {
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(_glfwWindow);

	_initializeWindowCallbacks();

	glfwSwapInterval(1);

    _elapsedTime = glfwGetTime();

	if (!_renderer) {
		_renderer = std::make_shared<Render::VulkanRenderer>();
	}

	_world->setRenderer(_renderer);
}

GlfwWindow::~GlfwWindow() {
	if (_glfwWindow != nullptr) {
		glfwDestroyWindow(_glfwWindow);
	}
}

void GlfwWindow::loopOnce() {
	if (_glfwWindow == nullptr)
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

bool GlfwWindow::shouldClose() const {
	return _glfwWindow == nullptr || glfwWindowShouldClose(_glfwWindow);
}

void GlfwWindow::_initializeWindowCallbacks() {
	glfwSetWindowUserPointer(_glfwWindow, this);

	glfwSetCursorPosCallback(_glfwWindow, [](GLFWwindow *win, double x, double y) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onMouseMoveCallback(x, y);
	});

	glfwSetMouseButtonCallback(_glfwWindow, [](GLFWwindow *win, int button, int action, int modifiers) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onMouseButtonCallback(button, action, modifiers);
	});

	glfwSetKeyCallback(_glfwWindow, [](GLFWwindow *win, int key, int scancode, int action, int mods) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onKeyCallback(key, scancode, action, mods);
	});

	glfwSetCharCallback(_glfwWindow, [](GLFWwindow *win, unsigned int codepoint) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onCharCallback(codepoint);
	});

	glfwSetWindowCloseCallback(_glfwWindow, [](GLFWwindow *win) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onCloseCallback();
	});

	glfwSetWindowSizeCallback(_glfwWindow, [](GLFWwindow *win, int width, int height) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		if (window->_glfwWindow == nullptr)
			return;
		glfwGetWindowSize(window->_glfwWindow, &(window->_settings.width), &(window->_settings.height));
		window->_onResizeCallback(width, height);
	});

	glfwSetScrollCallback(_glfwWindow, [](GLFWwindow *win, double x, double y) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onScrollCallback(x, y);
	});
}

} // namespace Stone::Window
