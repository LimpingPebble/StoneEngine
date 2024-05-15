// Copyright 2024 Stone-Engine

#include "Texture.hpp"

#include "../Device.hpp"
#include "../RenderContext.hpp"
#include "../RenderPass.hpp"
#include "../SwapChain.hpp"
#include "Scene/RenderElement/Texture.hpp"

namespace Stone::Render::Vulkan {

Texture::Texture(const std::shared_ptr<Scene::Texture> &texture, const std::shared_ptr<Device> &device,
				 const std::shared_ptr<RenderPass> &renderPass, const std::shared_ptr<SwapChain> &swapChain) {
}

Texture::~Texture() {
}

void Texture::render(Scene::RenderContext &context) {
	(void)context;
}

} // namespace Stone::Render::Vulkan
