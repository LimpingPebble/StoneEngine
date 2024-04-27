#include "config.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Window.hpp"
#include "Scene.hpp"

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    {
        auto app = std::make_shared<Stone::Window::App>();

        Stone::Window::WindowSettings win_settings;
        auto window = app->createWindow(win_settings);

        std::shared_ptr<Stone::Scene::WorldNode> world = std::make_shared<Stone::Scene::WorldNode>("root");
        window->setWorld(world);

        app->run();
    }

    std::cout << "Bye!" << std::endl;

#if STONE_ENGINE_USE_SYSTEM_PAUSE
    system("pause");
#endif

    return 0;
}
