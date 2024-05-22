#include "Physics/Physics.hpp"

#include <gtest/gtest.h>

using namespace Stone::Physics;

TEST(PhysicsEngineTest, UpdateSpherePosition) {
	Sphere sphere({0.0f, 0.0f, 0.0f}, 1.0f);
	PhysicsEngine physicsEngine;

	physicsEngine.applyForce(sphere, {1.0f, 0.0f, 0.0f});
	physicsEngine.update(sphere, 1.0f);

	EXPECT_EQ(sphere.center.x, 0.5f);
}

TEST(PhysicsEngineTest, DetectSphereSphereCollision) {
	Sphere sphere1({0.0f, 0.0f, 0.0f}, 1.0f);
	Sphere sphere2({2.0f, 0.0f, 0.0f}, 1.0f);
	glm::vec3 collisionPoint;
	PhysicsEngine physicsEngine;

	bool collisionDetected = physicsEngine.detectCollision(sphere1, sphere2, collisionPoint);

	EXPECT_TRUE(collisionDetected);
	EXPECT_EQ(collisionPoint.x, 1.0f);
	EXPECT_EQ(collisionPoint.y, 0.0f);
	EXPECT_EQ(collisionPoint.z, 0.0f);
}

TEST(PhysicsEngineTest, DetectSpherePlaneCollision) {
	Sphere sphere({0.0f, 0.0f, 2.0f}, 1.0f);
	Plane plane({0.0f, 0.0f, 1.0f}, 1.0f);
	glm::vec3 collisionPoint;
	PhysicsEngine physicsEngine;

	bool collisionDetected = physicsEngine.detectCollision(sphere, plane, collisionPoint);

	EXPECT_TRUE(collisionDetected);
	EXPECT_EQ(collisionPoint.x, 0.0f);
	EXPECT_EQ(collisionPoint.y, 0.0f);
	EXPECT_EQ(collisionPoint.z, 1.0f);
}
