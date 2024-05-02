// Copyright 2024 Stone-Engine

#include "Render/VulkanRenderer.hpp"
#include "Scene/ISceneRenderer.hpp"
#include "VulkanRendererObjectManager.hpp"

namespace Stone::Render {

void VulkanRenderer::updateDataForWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	std::shared_ptr<VulkanRendererObjectManager> manager =
		std::make_shared<VulkanRendererObjectManager>(std::dynamic_pointer_cast<VulkanRenderer>(shared_from_this()));
	world->traverseTopDown([this, manager](const std::shared_ptr<Scene::Node> &node) {
		auto renderElement = std::dynamic_pointer_cast<Scene::IRenderElement>(node);
		if (renderElement && renderElement->isDirty()) {
			renderElement->updateRenderObject(manager);
		}
	});
}

void VulkanRenderer::renderWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	vkWaitForFences(_device, 1, &_inFlightFence, VK_TRUE, UINT64_MAX);

	vkResetFences(_device, 1, &_inFlightFence);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(_commandBuffer, 0);

	recordCommandBuffer(_commandBuffer, imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {_imageAvailableSemaphore};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffer;

	VkSemaphore signalSemaphores[] = {_renderFinishedSemaphore};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {_swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(_presentQueue, &presentInfo);

	// Scene::RenderContext context;
	// world->initializeRenderContext(context);
	// world->render(context);
}

} // namespace Stone::Render
