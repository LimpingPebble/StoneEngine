
#pragma once

#include "Scene/Node/PivotNode.hpp"

class RotatingNode : public Stone::Scene::PivotNode {
	STONE_NODE(RotatingNode)

public:
	RotatingNode(const std::string &name = "rotating_node") : PivotNode(name) {
	}

	void update(float deltaTime) override {
		getTransform().rotate(deltaTime * rotationSpeeds);
	}

	void setRotationSpeed(glm::vec3 speeds) {
		rotationSpeeds = speeds;
	}

private:
	glm::vec3 rotationSpeeds = {0.0f, 0.4f, 0.0f};
};
