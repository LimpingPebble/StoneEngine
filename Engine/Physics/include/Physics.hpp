// Copyright 2024 Stone-Engine

#pragma once

#include <glm/glm.hpp>

namespace Stone::Physics {

using namespace Stone::Scene;

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float mass;

	Particle(const glm::vec3 &pos, float m) : position(pos), mass(m), velocity(0.0f), acceleration(0.0f) {
	}
};

class PhysicsEngine {
public:
	void update(Particle &particle, float deltaTime);
	void update(Sphere &sphere, float deltaTime);
	void applyForce(Particle &particle, const glm::vec3 &force);
	void applyForce(Sphere &sphere, const glm::vec3 &force);

	bool detectCollision(const Sphere &sphere1, const Sphere &sphere2, glm::vec3 &collisionPoint);
	bool detectCollision(const Sphere &sphere, const Plane &plane, glm::vec3 &collisionPoint);
	void resolveCollision(Sphere &sphere1, Sphere &sphere2, const glm::vec3 &collisionPoint);
	void resolveCollision(Sphere &sphere, const Plane &plane, const glm::vec3 &collisionPoint);

private:
	void integrate(Particle &particle, float deltaTime);
	void integrate(Sphere &sphere, float deltaTime);
};

} // namespace Stone::Physics
