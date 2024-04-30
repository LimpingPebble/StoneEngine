// Copyright 2024 Stone-Engine

#pragma once

#include "Window/WindowSettings.hpp"

namespace Stone::Scene {
class WorldNode;
}

namespace Stone::Window {

class App;

class Window : public std::enable_shared_from_this<Window> {
public:
	Window() = delete;
	Window(const std::shared_ptr<App> &app, WindowSettings settings);

	virtual ~Window();

	virtual void loopOnce();

	[[nodiscard]] virtual bool shouldClose() const;

	[[nodiscard]] const WindowSettings &getSettings() const;
	[[nodiscard]] std::shared_ptr<App> getApp() const;

	virtual void setWorld(std::shared_ptr<Stone::Scene::WorldNode> world);
	[[nodiscard]] std::shared_ptr<Stone::Scene::WorldNode> getWorld() const;

protected:
	void _onMouseMoveCallback(double x, double y);
	void _onMouseButtonCallback(int button, int action, int mods);
	void _onScrollCallback(double x, double y);
	void _onKeyCallback(int key, int scancode, int action, int mods);
	void _onCharCallback(unsigned int codepoint);
	void _onCloseCallback();
	void _onResizeCallback(int width, int height);

	std::weak_ptr<App> _app;
	WindowSettings _settings;
	std::shared_ptr<Stone::Scene::WorldNode> _world;
};

} // namespace Stone::Window
