#include "config.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Window.hpp"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    auto app = std::make_shared<Stone::Window::App>();

    Stone::Window::WindowSettings win_settings;
    auto window = app->createWindow(win_settings);

    app->run();

#if STONE_ENGINE_USE_SYSTEM_PAUSE
    system("pause");
#endif

	return 0;
}
