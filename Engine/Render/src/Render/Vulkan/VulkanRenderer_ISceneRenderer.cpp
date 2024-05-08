// Copyright 2024 Stone-Engine

#include "Render/Vulkan/VulkanRenderer.hpp"
#include "RendererObjectManager.hpp"
#include "Scene/ISceneRenderer.hpp"

namespace Stone::Render::Vulkan {

void VulkanRenderer::updateDataForWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	std::shared_ptr<RendererObjectManager> manager =
		std::make_shared<RendererObjectManager>(std::dynamic_pointer_cast<VulkanRenderer>(shared_from_this()));
	world->traverseTopDown([manager](const std::shared_ptr<Scene::Node> &node) {
		auto renderElement = std::dynamic_pointer_cast<Scene::IRenderElement>(node);
		if (renderElement && renderElement->isDirty()) {
			renderElement->updateRenderObject(manager);
		}
	});
}

void VulkanRenderer::renderWorld(const std::shared_ptr<Scene::WorldNode> &world) {

	// VkCommandBuffer &commandBuffer(_commandBuffers[_currentFrame]);
	// SyncronizedObjects &syncObject(_syncObjects[_currentFrame]);

	// vkWaitForFences(_device, 1, &syncObject.inFlight, VK_TRUE, UINT64_MAX);

	// uint32_t imageIndex;
	// VkResult result =
	// 	vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, syncObject.imageAvailable, VK_NULL_HANDLE, &imageIndex);

	// if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
	// 	std::cout << "Must recreate swap chain" << std::endl;
	// } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
	// 	throw std::runtime_error("failed to acquire swap chain image!");
	// }

	// vkResetFences(_device, 1, &syncObject.inFlight);

	// vkResetCommandBuffer(commandBuffer, 0);

	// recordCommandBuffer(commandBuffer, imageIndex);

	// VkSubmitInfo submitInfo{};
	// submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	// VkSemaphore waitSemaphores[] = {syncObject.imageAvailable};
	// VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	// submitInfo.waitSemaphoreCount = 1;
	// submitInfo.pWaitSemaphores = waitSemaphores;
	// submitInfo.pWaitDstStageMask = waitStages;
	// submitInfo.commandBufferCount = 1;
	// submitInfo.pCommandBuffers = &commandBuffer;

	// VkSemaphore signalSemaphores[] = {syncObject.renderFinished};
	// submitInfo.signalSemaphoreCount = 1;
	// submitInfo.pSignalSemaphores = signalSemaphores;

	// if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, syncObject.inFlight) != VK_SUCCESS) {
	// 	throw std::runtime_error("failed to submit draw command buffer!");
	// }

	// VkPresentInfoKHR presentInfo{};
	// presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	// presentInfo.waitSemaphoreCount = 1;
	// presentInfo.pWaitSemaphores = signalSemaphores;

	// VkSwapchainKHR swapChains[] = {_swapChain};
	// presentInfo.swapchainCount = 1;
	// presentInfo.pSwapchains = swapChains;
	// presentInfo.pImageIndices = &imageIndex;

	// vkQueuePresentKHR(_presentQueue, &presentInfo);

	(void)world;
	// // Scene::RenderContext context;
	// // world->initializeRenderContext(context);
	// // world->render(context);

	// if (_commandBuffers.empty()) {
	// 	return;
	// }

	// _currentFrame = (_currentFrame + 1) % _commandBuffers.size();
}

} // namespace Stone::Render::Vulkan
