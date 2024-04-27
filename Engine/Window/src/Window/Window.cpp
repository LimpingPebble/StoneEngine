// Copyright 2024 Stone-Engine

#include "Window/Window.hpp"

#include "Window/App.hpp"

#include <iostream>

namespace Stone::Window {

Window::Window(std::shared_ptr<App> app, const WindowSettings &settings)
	: std::enable_shared_from_this<Window>(), _app(app), _settings(settings) {
	std::cout << "window [" << this << "] created" << std::endl;
}

Window::~Window() {
	std::cout << "window [" << this << "] destroyed" << std::endl;
}

void Window::loopOnce() {
}

bool Window::shouldClose() const {
	return false;
}

const WindowSettings &Window::getSettings() const {
	return _settings;
}

std::shared_ptr<App> Window::getApp() const {
	return _app.lock();
}

void Window::setWorld(std::shared_ptr<Stone::Scene::WorldNode> world) {
	_world = world;
}

std::shared_ptr<Stone::Scene::WorldNode> Window::getWorld() const {
	return _world;
}

void Window::_onMouseMoveCallback(double x, double y) {
	std::cout << this << ":mouse move " << x << " " << y << std::endl;
}

void Window::_onMouseButtonCallback(int button, int action, int mods) {
	std::cout << this << ":mouse button " << button << " " << action << " " << mods << std::endl;
	if (button == 2 && action == 1) {
		_app.lock()->createWindow(_settings);
	}
}

void Window::_onScrollCallback(double x, double y) {
	std::cout << this << ":scroll " << x << " " << y << std::endl;
}

void Window::_onKeyCallback(int key, int scancode, int action, int mods) {
	std::cout << this << ":key " << key << " " << scancode << " " << action << " " << mods << std::endl;
}

void Window::_onCharCallback(unsigned int codepoint) {
	std::cout << this << ":char " << codepoint << std::endl;
}

void Window::_onCloseCallback() {
	std::cout << this << ":closed" << std::endl;
	getApp()->destroyWindow(shared_from_this());
}

void Window::_onResizeCallback(int width, int height) {
	std::cout << this << ":resize " << width << " " << height << std::endl;
}

} // namespace Stone::Window
