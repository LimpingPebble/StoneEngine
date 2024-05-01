// Copyright 2024 Stone-Engine

#pragma once

#include "Window/Window.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Stone::Window {

class GlfwWindow : public Window {
public:
	GlfwWindow() = delete;
	GlfwWindow(const std::shared_ptr<App> &app, const WindowSettings &settings);

	~GlfwWindow() override;

	void loopOnce() override;

	[[nodiscard]] bool shouldClose() const override;

	/** Window events */

private:
	GLFWwindow *_glfwWindow;

	void _initializeWindowCallbacks();
};

} // namespace Stone::Window
