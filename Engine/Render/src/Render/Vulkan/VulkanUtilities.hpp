// Copyright 2024 Stone-Engine

#pragma once

#include <cstring>
#include <iostream>
#include <limits>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

void enumerateExtensions(std::ostream &stream);
bool checkValidationLayerSupport(const std::vector<const char *> &validationLayers);

const char *to_string(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);
const char *to_string(VkDebugUtilsMessageTypeFlagsEXT messageType);

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily = {};
	std::optional<uint32_t> presentFamily = {};

	[[nodiscard]] bool isComplete() const {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities = {};
	std::vector<VkSurfaceFormatKHR> formats = {};
	std::vector<VkPresentModeKHR> presentModes = {};
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height);
