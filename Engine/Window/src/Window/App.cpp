// Copyright 2024 Stone-Engine

#include "Window/App.hpp"

#include "Window/GlfwWindow.hpp"
#include "Window/Window.hpp"

#include <algorithm>

namespace Stone::Window {

App::App() : std::enable_shared_from_this<App>(), _windows() {
	glfwInit();
}

App::~App() {
	glfwTerminate();
}

std::shared_ptr<Window> App::createWindow(const WindowSettings &settings) {
	std::shared_ptr<Window> win = std::make_shared<GlfwWindow>(shared_from_this(), settings);
	_windows.push_back(win);
	return win;
}

void App::destroyWindow(const std::shared_ptr<Window> &window) {
	auto it = std::find(_windows.begin(), _windows.end(), window);
	if (it != _windows.end()) {
		*it = nullptr;
	}
}

void App::run() {
	while (_windows.empty() == false) {
		for (int i = static_cast<int>(_windows.size()) - 1; i >= 0; --i) {
			if (_windows[i] == nullptr) {
				_windows.erase(_windows.begin() + i);
				continue;
			}

			_windows[i]->loopOnce();
		}
	}
}

} // namespace Stone::Window
