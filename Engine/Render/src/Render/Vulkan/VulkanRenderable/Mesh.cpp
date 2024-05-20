// Copyright 2024 Stone-Engine

#include "Mesh.hpp"

namespace Stone::Render::Vulkan {

Mesh::Mesh(const std::shared_ptr<Scene::Mesh> &mesh, const std::shared_ptr<VulkanRenderer> &renderer) {
	(void)mesh;
	(void)renderer;
}

Mesh::~Mesh() {
}

void Mesh::render(Scene::RenderContext &context) {
	assert(dynamic_cast<Vulkan::RenderContext *>(&context));
	auto vulkanContext = reinterpret_cast<Vulkan::RenderContext *>(&context);
}

} // namespace Stone::Render::Vulkan
