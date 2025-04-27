// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderer/RendererObjectFactory.hpp"

namespace Stone::Render::OpenGL {

class OpenGLRenderer;

class RendererObjectFactory : public Scene::RendererObjectFactory {

public:
	explicit RendererObjectFactory(std::shared_ptr<OpenGLRenderer> renderer);

	~RendererObjectFactory() override = default;

	void updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) override;
	void updateInstancedMeshNode(const std::shared_ptr<Scene::InstancedMeshNode> &instancedMeshNode) override;
	void updateSkinMeshNode(const std::shared_ptr<Scene::SkinMeshNode> &skinMeshNode) override;
	void updateMaterial(const std::shared_ptr<Scene::Material> &material) override;
	void updateDynamicMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh) override;
	void updateStaticMesh(const std::shared_ptr<Scene::StaticMesh> &mesh) override;
	void updateDynamicSkinMesh(const std::shared_ptr<Scene::DynamicSkinMesh> &skinmesh) override;
	void updateStaticSkinMesh(const std::shared_ptr<Scene::StaticSkinMesh> &skinmesh) override;
	void updateTexture(const std::shared_ptr<Scene::Texture> &texture) override;
	void updateFragmentShader(const std::shared_ptr<Scene::FragmentShader> &shader) override;


private:
	std::shared_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
