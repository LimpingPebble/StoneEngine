// Copyright 2024 Stone-Engine

#pragma once

#include "Mesh.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "Scene/Renderable/Mesh.hpp"
#include "Scene/Renderer/RendererDefaults.hpp"

#include <GL/glew.h>

template <typename T>
const std::shared_ptr<T> &first_non_null(const std::shared_ptr<T> &ptr) {
	return ptr;
}

template <typename T, typename... Args>
const std::shared_ptr<T> &first_non_null(const std::shared_ptr<T> &ptr, const Args &...args) {
	if (ptr != nullptr) {
		return ptr;
	}
	return first_non_null(args...);
}

namespace Stone::Render::OpenGL {

class MeshNode : public Scene::IRendererObject {
public:
	MeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _meshNode(meshNode), _renderer(renderer) {
	}

	~MeshNode() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
		assert(_meshNode.expired() == false);

		auto meshNode = _meshNode.lock();
		auto mesh = meshNode->getMesh();
		if (mesh == nullptr) {
			return;
		}

		auto rendererMesh = mesh->getRendererObject<RendererMesh>();
		if (rendererMesh == nullptr) {
			return;
		}

		auto material = first_non_null(meshNode->getMaterial(), mesh->getDefaultMaterial(), _renderer.lock()->getRendererDefaults()->getDefaultMaterial());

		const VRAMMesh &vramMesh = rendererMesh->getVRAMMesh();

		// TODO: Retrieve the corect shader program to use

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// TODO: Send uniforms from context to shader

		// Send material uniforms to program
	}

private:
	std::weak_ptr<Scene::MeshNode> _meshNode;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
