// Copyright 2024 Stone-Engine

#include "Render/VulkanRenderer.hpp"

#include "Render/VulkanDevice.hpp"

namespace Stone::Render {

VulkanRenderer::VulkanRenderer(VulkanSettings &settings) : Renderer() {
	std::cout << "VulkanRenderer created" << std::endl;

	_device = std::make_shared<VulkanDevice>(settings);

	// _createSwapChain(settings.frame_size);
	// _createImageViews();
	// _createRenderPass();
	// _createGraphicPipeline();
	// _createFramebuffers();
	// _createCommandBuffers();
	// _createSyncObjects();
}

VulkanRenderer::~VulkanRenderer() {
	if (_device) {
		_device->waitIdle();
	}

	// _destroySyncObjects();
	// _destroyCommandBuffers();
	// _destroyFramebuffers();
	// _destroyGraphicPipeline();
	// _destroyRenderPass();
	// _destroyImageViews();
	// _destroySwapChain();

	_device.reset();

	std::cout << "VulkanRenderer destroyed" << std::endl;
}

void VulkanRenderer::updateFrameSize(std::pair<uint32_t, uint32_t> size) {
	(void)size;
	// _recreateSwapChain(size);
}

/** Swap chain */

// void VulkanRenderer::_createSwapChain(const std::pair<uint32_t, uint32_t> &size) {
// 	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(_physicalDevice, _surface);

// 	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
// 	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

// 	auto [width, height] = size;
// 	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

// 	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
// 	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
// 		imageCount = swapChainSupport.capabilities.maxImageCount;
// 	}

// 	VkSwapchainCreateInfoKHR createInfo = {};
// 	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
// 	createInfo.surface = _surface;
// 	createInfo.minImageCount = imageCount;
// 	createInfo.imageFormat = surfaceFormat.format;
// 	createInfo.imageColorSpace = surfaceFormat.colorSpace;
// 	createInfo.imageExtent = extent;
// 	createInfo.imageArrayLayers = 1;
// 	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

// 	QueueFamilyIndices indices = findQueueFamilies(_physicalDevice, _surface);
// 	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

// 	if (indices.graphicsFamily != indices.presentFamily) {
// 		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
// 		createInfo.queueFamilyIndexCount = 2;
// 		createInfo.pQueueFamilyIndices = queueFamilyIndices;
// 	} else {
// 		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
// 		createInfo.queueFamilyIndexCount = 0;
// 		createInfo.pQueueFamilyIndices = nullptr;
// 	}

// 	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
// 	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
// 	createInfo.presentMode = presentMode;
// 	createInfo.clipped = VK_TRUE;
// 	createInfo.oldSwapchain = VK_NULL_HANDLE;

// 	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
// 		throw std::runtime_error("Failed to create swap chain");
// 	}

// 	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
// 	_swapChainImages.resize(imageCount);
// 	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());

// 	_swapChainImageFormat = surfaceFormat.format;
// 	_swapChainExtent = extent;
// }

// void VulkanRenderer::_destroySwapChain() {
// 	if (_swapChain == VK_NULL_HANDLE) {
// 		return;
// 	}
// 	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
// 	_swapChain = VK_NULL_HANDLE;
// }

// /** Image Views */

// void VulkanRenderer::_createImageViews() {
// 	_swapChainImageViews.resize(_swapChainImages.size());

// 	for (size_t i = 0; i < _swapChainImages.size(); ++i) {
// 		VkImageViewCreateInfo createInfo = {};
// 		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
// 		createInfo.image = _swapChainImages[i];

// 		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
// 		createInfo.format = _swapChainImageFormat;
// 		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
// 		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
// 		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
// 		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

// 		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		createInfo.subresourceRange.baseMipLevel = 0;
// 		createInfo.subresourceRange.levelCount = 1;
// 		createInfo.subresourceRange.baseArrayLayer = 0;
// 		createInfo.subresourceRange.layerCount = 1;

// 		if (vkCreateImageView(_device, &createInfo, nullptr, &_swapChainImageViews[i]) != VK_SUCCESS) {
// 			throw std::runtime_error("Failed to create image views");
// 		}
// 	}
// }

// void VulkanRenderer::_destroyImageViews() {
// 	for (auto imageView : _swapChainImageViews) {
// 		vkDestroyImageView(_device, imageView, nullptr);
// 	}
// 	_swapChainImageViews.clear();
// }

// void VulkanRenderer::_createRenderPass() {
// 	VkAttachmentDescription colorAttachment = {};
// 	colorAttachment.format = _swapChainImageFormat;
// 	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
// 	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
// 	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
// 	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
// 	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
// 	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// 	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

// 	VkAttachmentReference colorAttachmentRef = {};
// 	colorAttachmentRef.attachment = 0;
// 	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

// 	VkSubpassDescription subpass = {};
// 	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
// 	subpass.colorAttachmentCount = 1;
// 	subpass.pColorAttachments = &colorAttachmentRef;

// 	VkSubpassDependency dependency{};
// 	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
// 	dependency.dstSubpass = 0;
// 	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
// 	dependency.srcAccessMask = 0;
// 	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
// 	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

// 	VkRenderPassCreateInfo renderPassInfo = {};
// 	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
// 	renderPassInfo.attachmentCount = 1;
// 	renderPassInfo.pAttachments = &colorAttachment;
// 	renderPassInfo.subpassCount = 1;
// 	renderPassInfo.pSubpasses = &subpass;
// 	renderPassInfo.dependencyCount = 1;
// 	renderPassInfo.pDependencies = &dependency;

// 	if (vkCreateRenderPass(_device, &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
// 		throw std::runtime_error("Failed to create render pass");
// 	}
// }

// void VulkanRenderer::_destroyRenderPass() {
// 	if (_renderPass == VK_NULL_HANDLE) {
// 		return;
// 	}
// 	vkDestroyRenderPass(_device, _renderPass, nullptr);
// 	_renderPass = VK_NULL_HANDLE;
// }

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

// VkShaderModule VulkanRenderer::_createShaderModule(const std::vector<char> &code) {
// 	VkShaderModuleCreateInfo createInfo = {};
// 	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
// 	createInfo.codeSize = code.size();
// 	createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

// 	VkShaderModule shaderModule;
// 	if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
// 		throw std::runtime_error("Failed to create shader module");
// 	}

// 	return shaderModule;
// }

// void VulkanRenderer::_createFramebuffers() {
// 	_swapChainFramebuffers.resize(_swapChainImageViews.size());

// 	for (size_t i = 0; i < _swapChainFramebuffers.size(); ++i) {
// 		VkImageView attachments[] = {_swapChainImageViews[i]};

// 		VkFramebufferCreateInfo framebufferInfo = {};
// 		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
// 		framebufferInfo.renderPass = _renderPass;
// 		framebufferInfo.attachmentCount = 1;
// 		framebufferInfo.pAttachments = attachments;
// 		framebufferInfo.width = _swapChainExtent.width;
// 		framebufferInfo.height = _swapChainExtent.height;
// 		framebufferInfo.layers = 1;

// 		if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS) {
// 			throw std::runtime_error("Failed to create framebuffer");
// 		}
// 	}
// }

// void VulkanRenderer::_destroyFramebuffers() {
// 	for (auto framebuffer : _swapChainFramebuffers) {
// 		vkDestroyFramebuffer(_device, framebuffer, nullptr);
// 	}
// 	_swapChainFramebuffers.clear();
// }


// void VulkanRenderer::_recreateSwapChain(std::pair<uint32_t, uint32_t> size) {
// 	if (_device == VK_NULL_HANDLE) {
// 		return;
// 	}

// 	vkDeviceWaitIdle(_device);

// 	_destroyFramebuffers();
// 	_destroyImageViews();
// 	_destroySwapChain();

// 	_createSwapChain(size);
// 	_createImageViews();
// 	_createFramebuffers();
// }

// void VulkanRenderer::_createCommandBuffers() {

// 	_commandBuffers.resize(_swapChainFramebuffers.size());

// 	VkCommandBufferAllocateInfo allocInfo = {};
// 	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
// 	allocInfo.commandPool = _commandPool;
// 	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
// 	allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

// 	if (vkAllocateCommandBuffers(_device, &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
// 		throw std::runtime_error("Failed to allocate command buffer");
// 	}
// }

// void VulkanRenderer::_destroyCommandBuffers() {
// 	if (_commandBuffers.empty()) {
// 		return;
// 	}
// 	vkFreeCommandBuffers(_device, _commandPool, static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
// 	_commandBuffers.clear();
// }

// void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
// 	VkCommandBufferBeginInfo beginInfo = {};
// 	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

// 	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
// 		throw std::runtime_error("Failed to begin recording command buffer");
// 	}

// 	VkRenderPassBeginInfo renderPassInfo = {};
// 	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
// 	renderPassInfo.renderPass = _renderPass;
// 	renderPassInfo.framebuffer = _swapChainFramebuffers[imageIndex];
// 	renderPassInfo.renderArea.offset = {0, 0};
// 	renderPassInfo.renderArea.extent = _swapChainExtent;

// 	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
// 	renderPassInfo.clearValueCount = 1;
// 	renderPassInfo.pClearValues = &clearColor;

// 	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

// 	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);

// 	VkViewport viewport = {};
// 	viewport.x = 0.0f;
// 	viewport.y = 0.0f;
// 	viewport.width = static_cast<float>(_swapChainExtent.width);
// 	viewport.height = static_cast<float>(_swapChainExtent.height);
// 	viewport.minDepth = 0.0f;
// 	viewport.maxDepth = 1.0f;
// 	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

// 	VkRect2D scissor = {};
// 	scissor.offset = {0, 0};
// 	scissor.extent = _swapChainExtent;
// 	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

// 	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

// 	vkCmdEndRenderPass(commandBuffer);

// 	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
// 		throw std::runtime_error("Failed to record command buffer");
// 	}
// }

// void VulkanRenderer::_createSyncObjects() {
// 	_syncObjects.clear();
// 	_syncObjects.reserve(_commandBuffers.size());
// 	for (size_t i = 0; i < _commandBuffers.size(); ++i) {
// 		_syncObjects.emplace_back(_device);
// 	}
// }

// void VulkanRenderer::_destroySyncObjects() {
// 	_syncObjects.clear();
// }

// VulkanRenderer::SyncronizedObjects::SyncronizedObjects(VkDevice &device) : _device(device) {
// 	std::cout << "Creating syncronized objects" << std::endl;
// 	VkSemaphoreCreateInfo semaphoreInfo = {};
// 	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

// 	VkFenceCreateInfo fenceInfo = {};
// 	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
// 	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

// 	if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &imageAvailable) != VK_SUCCESS ||
// 		vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &renderFinished) != VK_SUCCESS ||
// 		vkCreateFence(_device, &fenceInfo, nullptr, &inFlight) != VK_SUCCESS) {
// 		throw std::runtime_error("Failed to create synchronization objects for a frame");
// 	}
// }

// VulkanRenderer::SyncronizedObjects::~SyncronizedObjects() {
// 	std::cout << "Destroying syncronized objects" << std::endl;
// 	if (imageAvailable != VK_NULL_HANDLE) {
// 		vkDestroySemaphore(_device, imageAvailable, nullptr);
// 	}
// 	if (renderFinished != VK_NULL_HANDLE) {
// 		vkDestroySemaphore(_device, renderFinished, nullptr);
// 	}
// 	if (inFlight != VK_NULL_HANDLE) {
// 		vkDestroyFence(_device, inFlight, nullptr);
// 	}
// }

} // namespace Stone::Render
