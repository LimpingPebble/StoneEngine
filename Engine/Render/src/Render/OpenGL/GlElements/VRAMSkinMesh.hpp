// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderable/SkinMesh.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

struct VRAMSkinMesh {

	VRAMSkinMesh(const std::shared_ptr<Scene::DynamicSkinMesh> &skinMesh) {
		if (skinMesh == nullptr) {
			return;
		}

		glGenVertexArrays(1, &elementsBuffer);
		if (elementsBuffer == 0) {
			throw std::runtime_error("Failed to generate vertex array buffer");
		}

		glGenBuffers(1, &verticesBuffer);
		if (verticesBuffer == 0) {
			glDeleteVertexArrays(1, &elementsBuffer);
			throw std::runtime_error("Failed to generate vertices buffer");
		}

		glGenBuffers(1, &indicesBuffer);
		if (indicesBuffer == 0) {
			glDeleteVertexArrays(1, &elementsBuffer);
			glDeleteBuffers(1, &verticesBuffer);
			throw std::runtime_error("Failed to generate indices buffer");
		}

		glBindVertexArray(elementsBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
		glBufferData(GL_ARRAY_BUFFER, skinMesh->getVertices().size() * sizeof(Scene::WeightVertex),
					 skinMesh->getVertices().data(), GL_STATIC_DRAW);

		numIndices = skinMesh->getIndices().size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), skinMesh->getIndices().data(),
					 GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Scene::WeightVertex),
							  (void *)offsetof(Scene::WeightVertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Scene::WeightVertex),
							  (void *)offsetof(Scene::WeightVertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Scene::WeightVertex),
							  (void *)offsetof(Scene::WeightVertex, tangent));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Scene::WeightVertex),
							  (void *)offsetof(Scene::WeightVertex, bitangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Scene::WeightVertex),
							  (void *)offsetof(Scene::WeightVertex, uv));

		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(
			5, 4, GL_INT, sizeof(Scene::WeightVertex),
			(void *)reinterpret_cast<std::size_t>(&reinterpret_cast<Scene::WeightVertex *>(0)->weights));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Scene::WeightVertex),
							  (void *)reinterpret_cast<std::size_t>(&reinterpret_cast<Scene::WeightVertex *>(0)->ids));
	}

	~VRAMSkinMesh() {
		if (verticesBuffer != 0) {
			glDeleteBuffers(1, &verticesBuffer);
		}
		if (indicesBuffer != 0) {
			glDeleteBuffers(1, &indicesBuffer);
		}
		if (elementsBuffer != 0) {
			glDeleteVertexArrays(1, &elementsBuffer);
		}
	}

	GLuint verticesBuffer = 0;
	GLuint indicesBuffer = 0;
	GLuint elementsBuffer = 0;
	GLsizei numIndices = 0;
};

} // namespace Stone::Render::OpenGL
