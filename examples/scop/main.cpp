#include "config.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Scene.hpp"
#include "Window.hpp"

class RotatingNode : public Stone::Scene::PivotNode {
public:
	STONE_NODE(RotatingNode)

	RotatingNode(const std::string &name = "rotating_node") : PivotNode(name) {
	}

	void update(float deltaTime) override {
		getTransform().rotate({0.0f, deltaTime * 0.1f, 0.0f});
		std::cout << "angle = " << getTransform().getEulerAngles().y << std::endl;
	}
};

STONE_NODE_IMPLEMENTATION(RotatingNode)

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	int retCode;
	{
		auto app = std::make_shared<Stone::Window::App>();

		Stone::Window::WindowSettings win_settings;
		win_settings.title = "Scop";
		auto window = app->createWindow(win_settings);

		auto meshNode = window->getWorld()->addChild<Stone::Scene::MeshNode>();

		auto mesh = std::make_shared<Stone::Scene::Mesh>();
		mesh->indicesRef() = {0, 1, 2, 0, 2, 3};
		mesh->verticesRef().emplace_back();
		mesh->verticesRef().back().position = {-0.5f, -0.5f, 0.0f};
		mesh->verticesRef().back().uv = {0.0f, 0.0f};
		mesh->verticesRef().emplace_back();
		mesh->verticesRef().back().position = {0.5f, -0.5f, 0.0f};
		mesh->verticesRef().back().uv = {1.0f, 0.0f};
		mesh->verticesRef().emplace_back();
		mesh->verticesRef().back().position = {0.5f, 0.5f, 0.0f};
		mesh->verticesRef().back().uv = {1.0f, 1.0f};
		mesh->verticesRef().emplace_back();
		mesh->verticesRef().back().position = {-0.5f, 0.5f, 0.0f};
		mesh->verticesRef().back().uv = {0.0f, 1.0f};

		meshNode->setMesh(mesh);

		auto rotatingNode = window->getWorld()->addChild<RotatingNode>();
		auto cameraNode = rotatingNode->addChild<Stone::Scene::PerspectiveCameraNode>();
		cameraNode->getTransform().setPosition({0.0f, 0.0f, -2.0f});

		window->getWorld()->setActiveCamera(cameraNode);

		retCode = app->run();
	}

	std::cout << "Bye!" << std::endl;

#if STONE_ENGINE_USE_SYSTEM_PAUSE
	system("pause");
#endif

	return retCode;
}
