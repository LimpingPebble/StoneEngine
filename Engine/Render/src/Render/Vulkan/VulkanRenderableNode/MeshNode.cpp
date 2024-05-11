// Copyright 2024 Stone-Engine

#include "MeshNode.hpp"

#include "../Device.hpp"
#include "../RenderContext.hpp"
#include "../RenderPass.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "Scene/RenderElement/Mesh.hpp"
#include "Utils/FileSystem.hpp"
#include "VertexBinding.hpp"

#include <cstring>
#include <stdexcept>

namespace Stone::Render::Vulkan {


MeshNode::MeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode, const std::shared_ptr<Device> &device,
				   const std::shared_ptr<RenderPass> &renderPass, VkExtent2D extent)
	: _device(device), _sceneMeshNode(meshNode) {
	_createGraphicPipeline(renderPass, extent);
	_createVertexBuffer();
}

MeshNode::~MeshNode() {
	_destroyVertexBuffer();
	_destroyGraphicPipeline();
}

void MeshNode::render(Scene::RenderContext &context) {
	auto vulkanContext = dynamic_cast<Vulkan::RenderContext *>(&context);
	assert(vulkanContext);

	vkCmdBindPipeline(vulkanContext->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicPipeline);

	VkBuffer vertexBuffers[] = {_vertexBuffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(vulkanContext->commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdDraw(vulkanContext->commandBuffer, _sceneMeshNode.lock()->getMesh()->getVertices().size(), 1, 0, 0);
}

void MeshNode::_createGraphicPipeline(const std::shared_ptr<RenderPass> &renderPass, VkExtent2D extent) {
	auto vertShaderCode = Utils::readFile("shaders/vert.spv");
	auto fragShaderCode = Utils::readFile("shaders/frag.spv");

	auto vertShaderModule = _device->createShaderModule(vertShaderCode);
	auto fragShaderModule = _device->createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

	auto bindingDescription = vertexBindingDescription<Scene::Vertex>();
	auto attributeDescriptions = vertexAttributeDescriptions<Scene::Vertex, 5>();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(extent.width);
	viewport.height = static_cast<float>(extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = extent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	if (vkCreatePipelineLayout(_device->getDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create pipeline layout");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = renderPass->getRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(_device->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicPipeline) !=
		VK_SUCCESS) {
		throw std::runtime_error("Failed to create graphics pipeline");
	}

	vkDestroyShaderModule(_device->getDevice(), vertShaderModule, nullptr);
	vkDestroyShaderModule(_device->getDevice(), fragShaderModule, nullptr);
}

void MeshNode::_destroyGraphicPipeline() {
	if (_graphicPipeline != VK_NULL_HANDLE) {
		vkDestroyPipeline(_device->getDevice(), _graphicPipeline, nullptr);
	}
	_graphicPipeline = VK_NULL_HANDLE;
	if (_pipelineLayout != VK_NULL_HANDLE) {
		vkDestroyPipelineLayout(_device->getDevice(), _pipelineLayout, nullptr);
	}
	_pipelineLayout = VK_NULL_HANDLE;
}

void MeshNode::_createVertexBuffer() {
	std::shared_ptr<Scene::MeshNode> meshNode = _sceneMeshNode.lock();
	assert(meshNode);

	const std::vector<Scene::Vertex> &vertices = meshNode->getMesh()->getVertices();

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	std::tie(_vertexBuffer, _vertexBufferMemory) =
		_device->createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
							  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void *data;
	vkMapMemory(_device->getDevice(), _vertexBufferMemory, 0, bufferSize, 0, &data);
	std::memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(_device->getDevice(), _vertexBufferMemory);
}

void MeshNode::_destroyVertexBuffer() {
	if (_vertexBuffer != VK_NULL_HANDLE) {
		vkDestroyBuffer(_device->getDevice(), _vertexBuffer, nullptr);
	}
	_vertexBuffer = VK_NULL_HANDLE;

	if (_vertexBufferMemory != VK_NULL_HANDLE) {
		vkFreeMemory(_device->getDevice(), _vertexBufferMemory, nullptr);
	}
	_vertexBufferMemory = VK_NULL_HANDLE;
}


} // namespace Stone::Render::Vulkan
