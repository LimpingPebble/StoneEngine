// Copyright 2024 StoneEngine

#pragma once

#include "Renderable/Shader.hpp"
#include "Scene/Renderer/RendererDefaults.hpp"

namespace Stone::Render::OpenGL {

class RendererDefaults : public Scene::RendererDefaults {
public:
	explicit RendererDefaults(const std::shared_ptr<OpenGLRenderer> &renderer);
	RendererDefaults(const RendererDefaults &other) = delete;

	~RendererDefaults() override = default;

	const std::shared_ptr<FragmentShader> &getFragmentShader() const {
		return _meshFragmentShader;
	}

	const std::shared_ptr<VertexShader> &getMeshVertexShader() const {
		return _meshVertexShader;
	}

	const std::shared_ptr<VertexShader> &getSkinMeshVertexShader() const {
		return _skinMeshVertexShader;
	}

	const std::shared_ptr<VertexShader> &getInstancedMeshVertexShader() const {
		return _instancedMeshVertexShader;
	}

protected:
	std::shared_ptr<FragmentShader> _meshFragmentShader;

	std::shared_ptr<VertexShader> _meshVertexShader;
	std::shared_ptr<VertexShader> _skinMeshVertexShader;
	std::shared_ptr<VertexShader> _instancedMeshVertexShader;
};

} // namespace Stone::Render::OpenGL
