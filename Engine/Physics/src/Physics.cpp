// Copyright 2024 Stone-Engine

#include "Physics/Physics.hpp"

namespace Stone::Physics {

void PhysicsEngine::update(Particle& particle, float deltaTime) {
    integrate(particle, deltaTime);
}

void PhysicsEngine::applyForce(Particle& particle, const glm::vec3& force) {
    particle.acceleration += force / particle.mass;
}

void PhysicsEngine::integrate(Particle& particle, float deltaTime) {
    // Update velocity
    particle.velocity += particle.acceleration * deltaTime;
    // Update position
    particle.position += particle.velocity * deltaTime;
    // Reset acceleration
    particle.acceleration = glm::vec3(0.0f);
}

} // namespace Stone::Physics
