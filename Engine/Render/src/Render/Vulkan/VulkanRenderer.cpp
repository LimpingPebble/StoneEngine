// Copyright 2024 Stone-Engine

#include "Render/Vulkan/VulkanRenderer.hpp"

#include "Device.hpp"
#include "FramesRenderer.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

namespace Stone::Render::Vulkan {

VulkanRenderer::VulkanRenderer(VulkanSettings &settings) : Renderer() {
	std::cout << "VulkanRenderer created" << std::endl;

	_device = std::make_shared<Device>(settings);

	SwapChainProperties swapChainProperties = _device->createSwapChainProperties(settings.frame_size);

	_renderPass = std::make_shared<RenderPass>(_device, swapChainProperties.surfaceFormat.format);
	_swapChain = std::make_shared<SwapChain>(_device, _renderPass->getRenderPass(), swapChainProperties);
	_framesRenderer = std::make_shared<FramesRenderer>(_device, _swapChain->getImagecount());
	assert(_framesRenderer->getImageCount() == _swapChain->getImagecount());
}

VulkanRenderer::~VulkanRenderer() {
	if (_device) {
		_device->waitIdle();
	}

	_framesRenderer.reset();
	_swapChain.reset();
	_renderPass.reset();
	_device.reset();

	std::cout << "VulkanRenderer destroyed" << std::endl;
}

void VulkanRenderer::updateFrameSize(std::pair<uint32_t, uint32_t> size) {
	_recreateSwapChain(size);
}

void VulkanRenderer::_recreateSwapChain(std::pair<uint32_t, uint32_t> size) {
	if (_device == nullptr) {
		return;
	}

	_device->waitIdle();

	_swapChain.reset();

	SwapChainProperties swapChainSettings = _device->createSwapChainProperties(size);
	_swapChain = std::make_shared<SwapChain>(_device, _renderPass->getRenderPass(), swapChainSettings);

	if (_framesRenderer == nullptr || _framesRenderer->getImageCount() != _swapChain->getImagecount()) {
		_framesRenderer.reset();
		_framesRenderer = std::make_shared<FramesRenderer>(_device, _swapChain->getImagecount());
	}

	assert(_framesRenderer->getImageCount() == _swapChain->getImagecount());
}


// void VulkanRenderer::_createGraphicPipeline() {
// 	auto vertShaderCode = Utils::readFile("shaders/vert.spv");
// 	auto fragShaderCode = Utils::readFile("shaders/frag.spv");

// 	auto vertShaderModule = _createShaderModule(vertShaderCode);
// 	auto fragShaderModule = _createShaderModule(fragShaderCode);

// 	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
// 	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
// 	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
// 	vertShaderStageInfo.module = vertShaderModule;
// 	vertShaderStageInfo.pName = "main";

// 	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
// 	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
// 	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
// 	fragShaderStageInfo.module = fragShaderModule;
// 	fragShaderStageInfo.pName = "main";

// 	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

// 	std::vector<VkDynamicState> dynamicStates = {
// 		VK_DYNAMIC_STATE_VIEWPORT,
// 		VK_DYNAMIC_STATE_SCISSOR,
// 	};

// 	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
// 	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
// 	dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
// 	dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

// 	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
// 	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
// 	vertexInputInfo.vertexBindingDescriptionCount = 0;
// 	vertexInputInfo.vertexAttributeDescriptionCount = 0;

// 	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
// 	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
// 	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
// 	inputAssembly.primitiveRestartEnable = VK_FALSE;

// 	VkViewport viewport = {};
// 	viewport.x = 0.0f;
// 	viewport.y = 0.0f;
// 	viewport.width = static_cast<float>(_swapChainExtent.width);
// 	viewport.height = static_cast<float>(_swapChainExtent.height);
// 	viewport.minDepth = 0.0f;
// 	viewport.maxDepth = 1.0f;

// 	VkRect2D scissor = {};
// 	scissor.offset = {0, 0};
// 	scissor.extent = _swapChainExtent;

// 	VkPipelineViewportStateCreateInfo viewportState = {};
// 	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
// 	viewportState.viewportCount = 1;
// 	viewportState.pViewports = &viewport;
// 	viewportState.scissorCount = 1;
// 	viewportState.pScissors = &scissor;

// 	VkPipelineRasterizationStateCreateInfo rasterizer = {};
// 	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
// 	rasterizer.depthClampEnable = VK_FALSE;
// 	rasterizer.rasterizerDiscardEnable = VK_FALSE;
// 	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
// 	rasterizer.lineWidth = 1.0f;
// 	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
// 	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
// 	rasterizer.depthBiasEnable = VK_FALSE;
// 	rasterizer.depthBiasConstantFactor = 0.0f;
// 	rasterizer.depthBiasClamp = 0.0f;
// 	rasterizer.depthBiasSlopeFactor = 0.0f;

// 	VkPipelineMultisampleStateCreateInfo multisampling = {};
// 	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
// 	multisampling.sampleShadingEnable = VK_FALSE;
// 	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
// 	multisampling.minSampleShading = 1.0f;
// 	multisampling.pSampleMask = nullptr;
// 	multisampling.alphaToCoverageEnable = VK_FALSE;
// 	multisampling.alphaToOneEnable = VK_FALSE;

// 	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
// 	colorBlendAttachment.colorWriteMask =
// 		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
// 	colorBlendAttachment.blendEnable = VK_TRUE;
// 	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
// 	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
// 	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
// 	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
// 	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
// 	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

// 	VkPipelineColorBlendStateCreateInfo colorBlending = {};
// 	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
// 	colorBlending.logicOpEnable = VK_FALSE;
// 	colorBlending.logicOp = VK_LOGIC_OP_COPY;
// 	colorBlending.attachmentCount = 1;
// 	colorBlending.pAttachments = &colorBlendAttachment;
// 	colorBlending.blendConstants[0] = 0.0f;
// 	colorBlending.blendConstants[1] = 0.0f;
// 	colorBlending.blendConstants[2] = 0.0f;
// 	colorBlending.blendConstants[3] = 0.0f;

// 	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
// 	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
// 	pipelineLayoutInfo.setLayoutCount = 0;
// 	pipelineLayoutInfo.pushConstantRangeCount = 0;

// 	if (vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
// 		throw std::runtime_error("Failed to create pipeline layout");
// 	}

// 	VkGraphicsPipelineCreateInfo pipelineInfo = {};
// 	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
// 	pipelineInfo.stageCount = 2;
// 	pipelineInfo.pStages = shaderStages;
// 	pipelineInfo.pVertexInputState = &vertexInputInfo;
// 	pipelineInfo.pInputAssemblyState = &inputAssembly;
// 	pipelineInfo.pViewportState = &viewportState;
// 	pipelineInfo.pRasterizationState = &rasterizer;
// 	pipelineInfo.pMultisampleState = &multisampling;
// 	pipelineInfo.pDepthStencilState = nullptr;
// 	pipelineInfo.pColorBlendState = &colorBlending;
// 	pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
// 	pipelineInfo.layout = _pipelineLayout;
// 	pipelineInfo.renderPass = _renderPass;
// 	pipelineInfo.subpass = 0;
// 	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
// 	pipelineInfo.basePipelineIndex = -1;

// 	if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) !=
// 		VK_SUCCESS) {
// 		throw std::runtime_error("Failed to create graphics pipeline");
// 	}

// 	vkDestroyShaderModule(_device, vertShaderModule, nullptr);
// 	vkDestroyShaderModule(_device, fragShaderModule, nullptr);
// }

// void VulkanRenderer::_destroyGraphicPipeline() {
// 	if (_graphicsPipeline != VK_NULL_HANDLE) {
// 		vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
// 	}
// 	_graphicsPipeline = VK_NULL_HANDLE;
// 	if (_pipelineLayout != VK_NULL_HANDLE) {
// 		vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
// 	}
// 	_pipelineLayout = VK_NULL_HANDLE;
// }

} // namespace Stone::Render::Vulkan
