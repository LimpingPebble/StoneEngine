
#pragma once

#include "Core/Assets/Bundle.hpp"
#include "Core/Image/ImageSource.hpp"
#include "RotatingNode.hpp"
#include "Scene.hpp"
#include "Scene/Assets/AssetResource.hpp"
#include "Scene/Node/WorldNode.hpp"

namespace Scop {

class SceneController {
public:
	SceneController(std::shared_ptr<Stone::Scene::WorldNode> world) : world(world) {
		assetsBundle = std::make_shared<Stone::Core::Assets::Bundle>();

		// Generate a Mesh
		auto mesh = makePlaneMesh();

		// Create a MeshNode
		auto meshNode = std::make_shared<Stone::Scene::MeshNode>();
		world->addChild(meshNode);
		meshNode->setMesh(mesh);

		// Create a Texture
		auto stone_texture = std::make_shared<Stone::Scene::Texture>();
		auto stone_image_source = assetsBundle->loadResource<Stone::Core::Image::ImageSource>(
			"docs/img/stone-engine.png", Stone::Core::Image::Channel::RGBA);
		stone_texture->setImage(stone_image_source);

		// Create a Material using the texture
		auto stone_material = std::make_shared<Stone::Scene::Material>();
		stone_material->setTextureParameter("diffuse", stone_texture);
		meshNode->setMaterial(stone_material);

		// Create a shader from a file
		// auto stone_shader = std::make_shared<Stone::Scene::FragmentShader>("shaders/frag.spv");
		// stone_shader->setLocation("diffuse", 1);
		// stone_material->setFragmentShader(stone_shader);

		// Create a second MeshNode with the same mesh
		auto meshRotatingNode = world->addChild<RotatingNode>();
		meshRotatingNode->getTransform().setPosition({0.0f, 0.0f, 0.0f});
		auto secondMeshNode = std::make_shared<Stone::Scene::MeshNode>();
		meshRotatingNode->addChild(secondMeshNode);
		meshRotatingNode->setRotationSpeed({0.0f, 0.4f, 0.0f});
		secondMeshNode->setMesh(mesh);

		// Create a blue material that takes no texture as everything is in the shader code
		// auto blueShader = std::make_shared<Stone::Scene::FragmentShader>("shaders/frag-blue.glsl");
		// auto blueMaterial = std::make_shared<Stone::Scene::Material>();
		// blueMaterial->setFragmentShader(blueShader);
		// secondMeshNode->setMaterial(stone_material /* blueMaterial */);

		// Create a camera moving around the scene
		auto cameraRotator = world->addChild<RotatingNode>();
		auto cameraNode = cameraRotator->addChild<Stone::Scene::PerspectiveCameraNode>();
		cameraNode->getTransform().setPosition({0.0f, 3.0f, 3.0f});
		cameraNode->getTransform().rotate({-0.6f, 0.0f, 0.0f});
		world->setActiveCamera(cameraNode);
	}

	void loadAsset(std::string path) {
		// Load the asset from the given path
		assetsBundle = std::make_shared<Stone::Core::Assets::Bundle>();
		auto asset = assetsBundle->loadResource<Stone::Scene::AssetResource>(path);
		auto node = asset->getRootNode();
		world->addChild(node);
		std::cout << asset->getMetadatas() << std::endl;
		node->writeHierarchy(std::cout);
	}

private:
	std::shared_ptr<Stone::Scene::DynamicMesh> makePlaneMesh() {
		auto mesh = std::make_shared<Stone::Scene::DynamicMesh>();
		mesh->withElementsRef([](auto vertices, auto indices) {
			indices = {0, 1, 2, 0, 2, 3};
			vertices.emplace_back();
			vertices.back().position = {-0.5f, -0.5f, 0.0f};
			vertices.back().uv = {0.0f, 0.0f};
			vertices.emplace_back();
			vertices.back().position = {0.5f, -0.5f, 0.0f};
			vertices.back().uv = {1.0f, 0.0f};
			vertices.emplace_back();
			vertices.back().position = {0.5f, 0.5f, 0.0f};
			vertices.back().uv = {1.0f, 1.0f};
			vertices.emplace_back();
			vertices.back().position = {-0.5f, 0.5f, 0.0f};
			vertices.back().uv = {0.0f, 1.0f};
		});
		return mesh;
	}

	std::shared_ptr<Stone::Scene::WorldNode> world;
	std::shared_ptr<Stone::Core::Assets::Bundle> assetsBundle;
};

} // namespace Scop
