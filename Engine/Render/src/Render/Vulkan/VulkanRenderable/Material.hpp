// Copyright 2024 Stone-Engine

#pragma once

#include "../RenderContext.hpp"
#include "Scene/RenderElement/IRenderable.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace Stone::Scene {
class Material;
} // namespace Stone::Scene

namespace Stone::Render::Vulkan {

class VulkanRenderer;
class Device;
class RenderPass;
class SwapChain;

class Material : public Scene::IRendererObject {
public:
	Material(const std::shared_ptr<Scene::Material> &material, const std::shared_ptr<VulkanRenderer> &renderer);

	~Material() override;

	void render(Scene::RenderContext &context) override;
};

} // namespace Stone::Render::Vulkan
