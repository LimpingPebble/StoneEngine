// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/RenderElement/IRenderElement.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Scene {
class Shader;
} // namespace Stone::Scene

namespace Stone::Render::Vulkan {

class VulkanRenderer;

class Shader : public Scene::IRendererObject {
public:
	Shader(const std::shared_ptr<Scene::Shader> &shader, const std::shared_ptr<VulkanRenderer> &renderer);
	~Shader() override;

	void render(Scene::RenderContext &context) override;
};

} // namespace Stone::Render::Vulkan
