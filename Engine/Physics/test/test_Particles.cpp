#include "Physics/Physics.hpp"

#include <gtest/gtest.h>

using namespace Stone::Physics;

TEST(ParticleTest, UpdateParticlePosition) {
	Particle particle({0.0f, 0.0f, 0.0f}, 1.0f);
	PhysicsEngine physicsEngine;

	physicsEngine.applyForce(particle, {1.0f, 0.0f, 0.0f});
	physicsEngine.update(particle, 1.0f);

	EXPECT_EQ(particle.position.x, 0.5f);
}
