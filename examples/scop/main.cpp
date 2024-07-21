#include "config.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Core/Image/ImageSource.hpp"
#include "Scene.hpp"
#include "Window.hpp"

class RotatingNode : public Stone::Scene::PivotNode {
public:
	STONE_NODE(RotatingNode)

	RotatingNode(const std::string &name = "rotating_node") : PivotNode(name) {
	}

	void update(float deltaTime) override {
		getTransform().rotate(deltaTime * rotationSpeeds);
	}

	void setRotationSpeed(glm::vec3 speeds) {
		rotationSpeeds = speeds;
	}

private:
	glm::vec3 rotationSpeeds = {0.0f, 0.4f, 0.0f};
};

STONE_NODE_IMPLEMENTATION(RotatingNode)

int main(int argc, char **argv) {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	int retCode;
	{
		// Create an App
		auto app = std::make_shared<Stone::Window::App>();

		// Create a Window
		Stone::Window::WindowSettings win_settings;
		win_settings.title = "Scop";
		auto window = app->createWindow(win_settings);

		// Generate a Mesh
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

		// Create a MeshNode
		auto meshNode = window->getWorld()->addChild<Stone::Scene::MeshNode>();
		meshNode->setMesh(mesh);

		// Create a Texture
		auto stone_texture = std::make_shared<Stone::Scene::Texture>();
		auto stone_image_source =
			std::make_shared<Stone::Image::ImageSource>("docs/img/stone-engine.png", Stone::Image::Channel::RGBA);
		stone_texture->setImage(stone_image_source);

		// Create a Material using the texture
		auto stone_material = std::make_shared<Stone::Scene::Material>();
		stone_material->setTextureParameter("diffuse", stone_texture);
		meshNode->setMaterial(stone_material);

		// Create a shader from a file
		auto stone_shader = std::make_shared<Stone::Scene::Shader>("shaders/frag.spv");
		stone_shader->setLocation("diffuse", 1);
		stone_material->setFragmentShader(stone_shader);

		// Create a second MeshNode with the same mesh
		auto meshRotatingNode = window->getWorld()->addChild<RotatingNode>();
		meshRotatingNode->getTransform().setPosition({0.0f, 0.0f, 0.0f});
		auto secondMeshNode = meshRotatingNode->addChild<Stone::Scene::MeshNode>();
		meshRotatingNode->setRotationSpeed({0.0f, 0.4f, 0.0f});
		secondMeshNode->setMesh(mesh);

		// Create a blue material that takes no texture as everything is in the shader code
		auto blueShader = std::make_shared<Stone::Scene::Shader>("shaders/frag-blue.glsl");
		auto blueMaterial = std::make_shared<Stone::Scene::Material>();
		blueMaterial->setFragmentShader(blueShader);
		secondMeshNode->setMaterial(stone_material /* blueMaterial */);

		// Create a camera moving around the scene
		auto cameraRotator = window->getWorld()->addChild<RotatingNode>();
		auto cameraNode = cameraRotator->addChild<Stone::Scene::PerspectiveCameraNode>();
		cameraNode->getTransform().setPosition({0.0f, 3.0f, 3.0f});
		cameraNode->getTransform().rotate({-0.6f, 0.0f, 0.0f});
		window->getWorld()->setActiveCamera(cameraNode);

        // Load a node from a file
        if (argc > 1) {
            auto node = Stone::Scene::Node::load(argv[1]);
            window->getWorld()->addChild(node);
            node->writeHierarchy(std::cout);
        }

		// Run the App
		retCode = app->run();
	}

	std::cout << "Bye!" << std::endl;

#if STONE_ENGINE_USE_SYSTEM_PAUSE
	system("pause");
#endif

	return retCode;
}
