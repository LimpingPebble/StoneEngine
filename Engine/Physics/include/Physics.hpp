// Copyright 2024 Stone-Engine

#pragma once

#include <glm/glm.hpp>

namespace Stone::Physics {

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;

    Particle(const glm::vec3& pos, float m)
        : position(pos), mass(m), velocity(0.0f), acceleration(0.0f) {}
};

class PhysicsEngine {
public:
    void update(Particle& particle, float deltaTime);
    void applyForce(Particle& particle, const glm::vec3& force);

private:
    void integrate(Particle& particle, float deltaTime);
};

} // namespace Stone::Physics
