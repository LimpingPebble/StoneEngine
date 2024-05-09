// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Vertex.hpp"

#include <array>
#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

template <typename T>
VkVertexInputBindingDescription vertexBindingDescription() {
	static_assert(std::is_same_v<T, Scene::Vertex> || std::is_same_v<T, Scene::WeightVertex>,
				  "Unsupported vertex type");
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(T);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

template <typename T, std::size_t N>
std::array<VkVertexInputAttributeDescription, N> vertexAttributeDescriptions() {
	static_assert(std::is_same_v<T, Scene::Vertex> || std::is_same_v<T, Scene::WeightVertex>,
				  "Unsupported vertex type");
	return {};
}

template <>
std::array<VkVertexInputAttributeDescription, 5> vertexAttributeDescriptions<Scene::Vertex, 5>() {
	std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions = {};

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Scene::Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Scene::Vertex, normal);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Scene::Vertex, tangent);

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(Scene::Vertex, bitangent);

	attributeDescriptions[4].binding = 0;
	attributeDescriptions[4].location = 4;
	attributeDescriptions[4].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[4].offset = offsetof(Scene::Vertex, uv);

	return attributeDescriptions;
}

template <>
std::array<VkVertexInputAttributeDescription, 7> vertexAttributeDescriptions<Scene::WeightVertex, 7>() {
	std::array<VkVertexInputAttributeDescription, 7> attributeDescriptions = {};

	std::array<VkVertexInputAttributeDescription, 5> baseDescriptions = vertexAttributeDescriptions<Scene::Vertex, 5>();
	for (std::size_t i = 0; i < baseDescriptions.size(); ++i) {
		attributeDescriptions[i] = baseDescriptions[i];
	}

	attributeDescriptions[5].binding = 0;
	attributeDescriptions[5].location = 5;
	attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[5].offset =
		reinterpret_cast<std::size_t>(&reinterpret_cast<Scene::WeightVertex *>(0)->weights);

	attributeDescriptions[6].binding = 0;
	attributeDescriptions[6].location = 6;
	attributeDescriptions[6].format = VK_FORMAT_R32G32B32A32_SINT;
	attributeDescriptions[6].offset = reinterpret_cast<std::size_t>(&reinterpret_cast<Scene::WeightVertex *>(0)->ids);

	return attributeDescriptions;
}

} // namespace Stone::Render::Vulkan
