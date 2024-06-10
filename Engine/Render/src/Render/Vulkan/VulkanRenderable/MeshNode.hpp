// Copyright 2024 Stone-Engine

#pragma once

#include "../RenderContext.hpp"
#include "Scene/Renderable/IRenderable.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace Stone::Scene {
class MeshNode;
} // namespace Stone::Scene

namespace Stone::Render::Vulkan {

class VulkanRenderer;
class Device;
class RenderPass;
class SwapChain;

class MeshNode : public Scene::IRendererObject {
public:
	MeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode, const std::shared_ptr<VulkanRenderer> &renderer);

	~MeshNode() override;

	void render(Scene::RenderContext &context) override;

private:
	void _updateUniformBuffers(Vulkan::RenderContext &context);

	void _createDescriptorSetLayout();
	void _destroyDescriptorSetLayout();

	void _createGraphicPipeline(const std::shared_ptr<RenderPass> &renderPass, VkExtent2D extent);
	void _destroyGraphicPipeline();

	void _createVertexBuffer();
	void _destroyVertexBuffer();

	void _createIndexBuffer();
	void _destroyIndexBuffer();

	void _createUniformBuffers(const std::shared_ptr<SwapChain> &swapChain);
	void _destroyUniformBuffers();

	void _createDescriptorPool(const std::shared_ptr<SwapChain> &swapChain);
	void _destroyDescriptorPool();

	void _createDescriptorSets(const std::shared_ptr<SwapChain> &swapChain);
	void _destroyDescriptorSets();

	std::shared_ptr<Device> _device;

	std::weak_ptr<Scene::MeshNode> _sceneMeshNode;

	VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

	VkPipeline _graphicPipeline = VK_NULL_HANDLE;
	VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;

	VkBuffer _vertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory _vertexBufferMemory = VK_NULL_HANDLE;
	VkBuffer _indexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory _indexBufferMemory = VK_NULL_HANDLE;
	// TODO: Use only one buffer for vertices and indices and use offsets

	std::vector<VkBuffer> _uniformBuffers;
	std::vector<VkDeviceMemory> _uniformBuffersMemory;
	std::vector<void *> _uniformBuffersMapped;

	VkDescriptorPool _descriptorPool = VK_NULL_HANDLE;
	std::vector<VkDescriptorSet> _descriptorSets;
};

} // namespace Stone::Render::Vulkan
