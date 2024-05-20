// Copyright 2024 Stone-Engine

#include "Shader.hpp"

namespace Stone::Render::Vulkan {

Shader::Shader(const std::shared_ptr<Scene::Shader> &shader, const std::shared_ptr<VulkanRenderer> &renderer) {
	(void)shader;
	(void)renderer;
}

Shader::~Shader() {
}

void Shader::render(Scene::RenderContext &context) {
	(void)context;
}

} // namespace Stone::Render::Vulkan
