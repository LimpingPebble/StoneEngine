#include "Scene/Transform.hpp"

#include <gtest/gtest.h>

using namespace Stone::Scene;

TEST(Transform, Transform2D) {
	Transform2D transform;

	// Set position
	transform.setPosition(glm::vec2(1.0f, 2.0f));
	EXPECT_EQ(transform.getPosition(), glm::vec2(1.0f, 2.0f));

	// Set rotation
	transform.setRotation(0.5f);
	EXPECT_EQ(transform.getRotation(), 0.5f);

	// Set scale
	transform.setScale(glm::vec2(2.0f, 2.0f));
	EXPECT_EQ(transform.getScale(), glm::vec2(2.0f, 2.0f));

	// Translate
	transform.translate(glm::vec2(1.0f, 1.0f));
	EXPECT_EQ(transform.getPosition(), glm::vec2(2.0f, 3.0f));

	// Rotate
	transform.rotate(0.5f);
	EXPECT_EQ(transform.getRotation(), 1.0f);

	// Scale
	transform.scale(glm::vec2(2.0f, 2.0f));
	EXPECT_EQ(transform.getScale(), glm::vec2(4.0f, 4.0f));
}

TEST(Transform, Transform3D) {
	Transform3D transform;

	// Set position
	transform.setPosition(glm::vec3(1.0f, 2.0f, 3.0f));
	EXPECT_EQ(transform.getPosition(), glm::vec3(1.0f, 2.0f, 3.0f));

	// Set rotation
	transform.setRotation(glm::quat(glm::vec3(0.1f, 0.2f, 0.3f)));
	EXPECT_EQ(transform.getRotation(), glm::quat(glm::vec3(0.1f, 0.2f, 0.3f)));

	// Set scale
	transform.setScale(glm::vec3(2.0f, 2.0f, 2.0f));
	EXPECT_EQ(transform.getScale(), glm::vec3(2.0f, 2.0f, 2.0f));

	// Translate
	transform.translate(glm::vec3(1.0f, 1.0f, 1.0f));
	EXPECT_EQ(transform.getPosition(), glm::vec3(2.0f, 3.0f, 4.0f));

	// Rotate
	// transform.rotate(glm::vec3(0.1f, 0.2f, 0.3f));
	// EXPECT_EQ(transform.getRotation(), glm::quat(glm::vec3(0.2f, 0.4f, 0.6f)));

	// Scale
	transform.scale(glm::vec3(2.0f, 2.0f, 2.0f));
	EXPECT_EQ(transform.getScale(), glm::vec3(4.0f, 4.0f, 4.0f));

	// Euler angles
	transform.setEulerAngles(glm::vec3(0.1f, 0.2f, 0.3f));
	EXPECT_EQ(transform.getEulerAngles(), glm::vec3(0.1f, 0.2f, 0.3f));
}
