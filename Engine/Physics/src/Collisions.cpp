// Copyright 2024 Stone-Engine

#include "Physics/Physics.hpp"

namespace Stone::Physics {

void PhysicsEngine::update(Stone::Scene::Sphere &sphere, float deltaTime) {
	integrate(sphere, deltaTime);
}

void PhysicsEngine::applyForce(Stone::Scene::Sphere &sphere, const glm::vec3 &force) {
	sphere.acceleration += force / sphere.mass;
}

void PhysicsEngine::integrate(Stone::Scene::Sphere &sphere, float deltaTime) {
	sphere.velocity += sphere.acceleration * deltaTime;
	sphere.center += sphere.velocity * deltaTime;
	sphere.acceleration = glm::vec3(0.0f);
}

bool PhysicsEngine::detectCollision(const Stone::Scene::Sphere &sphere1, const Stone::Scene::Sphere &sphere2,
									glm::vec3 &collisionPoint) {
	glm::vec3 delta = sphere2.center - sphere1.center;
	float distance = glm::length(delta);
	if (distance < sphere1.radius + sphere2.radius) {
		collisionPoint = sphere1.center + glm::normalize(delta) * sphere1.radius;
		return true;
	}
	return false;
}

bool PhysicsEngine::detectCollision(const Stone::Scene::Sphere &sphere, const Stone::Scene::Plane &plane,
									glm::vec3 &collisionPoint) {
	float distanceFromPlane = glm::dot(sphere.center, plane.normal) - plane.distance;
	if (distanceFromPlane < sphere.radius) {
		collisionPoint = sphere.center - plane.normal * distanceFromPlane;
		return true;
	}
	return false;
}

void PhysicsEngine::resolveCollision(Stone::Scene::Sphere &sphere1, Stone::Scene::Sphere &sphere2,
									 const glm::vec3 &collisionPoint) {
	glm::vec3 collisionNormal = glm::normalize(sphere2.center - sphere1.center);
	glm::vec3 relativeVelocity = sphere2.velocity - sphere1.velocity;
	float normalVelocity = glm::dot(relativeVelocity, collisionNormal);

	if (normalVelocity < 0.0f) {
		float restitution = 0.5f; // Coefficient of restitution
		glm::vec3 impulse = collisionNormal * (-1.5f * normalVelocity / (1 / sphere1.mass + 1 / sphere2.mass));

		sphere1.velocity += impulse / sphere1.mass;
		sphere2.velocity -= impulse / sphere2.mass;

		float penetrationDepth = sphere1.radius + sphere2.radius - glm::length(sphere2.center - sphere1.center);
		sphere1.center -= collisionNormal * (penetrationDepth / 2.0f);
		sphere2.center += collisionNormal * (penetrationDepth / 2.0f);
	}
}

void PhysicsEngine::resolveCollision(Stone::Scene::Sphere &sphere, const Stone::Scene::Plane &plane,
									 const glm::vec3 &collisionPoint) {
	glm::vec3 relativeVelocity = sphere.velocity;
	float normalVelocity = glm::dot(relativeVelocity, plane.normal);

	if (normalVelocity < 0.0f) {
		float restitution = 0.5f; // Coefficient of restitution
		glm::vec3 impulse = plane.normal * (-1.5f * normalVelocity * sphere.mass);

		sphere.velocity += impulse / sphere.mass;
		sphere.center = collisionPoint + plane.normal * sphere.radius;
	}
}

} // namespace Stone::Physics
