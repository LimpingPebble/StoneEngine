
#pragma once

#include "ScopSceneController.hpp"
#include "Window.hpp"

#include <filesystem>

namespace Scop {

class Env {
public:
	Env(std::optional<std::string> path = std::nullopt) {
		std::cout << "Starting in directory " << std::filesystem::current_path() << std::endl;

		app = std::make_shared<Stone::Window::App>();

		_makeWindow();
		_makeSceneController();

		if (path)
			sceneController->loadAsset(*path);
	}

	int run() {
		return app->run();
	}

	~Env() {
		std::cout << "Bye!" << std::endl;
	}

private:
	void _makeWindow() {
		Stone::Window::WindowSettings win_settings;
		win_settings.title = "Scop";
		win_settings.width = 1280;
		win_settings.height = 720;
		win_settings.resizable = true;

		window = app->createWindow(win_settings);
	}

	void _makeSceneController() {
		sceneController = std::make_shared<SceneController>(window->getWorld());
	}

	std::shared_ptr<Stone::Window::App> app;
	std::shared_ptr<Stone::Window::Window> window;
	std::shared_ptr<SceneController> sceneController;
};

} // namespace Scop
