// Copyright 2024 Stone-Engine

#pragma once

#include "Window/WindowSettings.hpp"

namespace Stone::Window {

class Window;

class App : public std::enable_shared_from_this<App> {
public:
	App();

	virtual ~App();

	/** Create a window. Can be called before or during the application loop */
	std::shared_ptr<Window> createWindow(const WindowSettings &settings);

	void destroyWindow(std::shared_ptr<Window> window);

	/** Run the application loop */
	void run();

private:
	std::vector<std::shared_ptr<Window>> _windows;
};

} // namespace Stone::Window
