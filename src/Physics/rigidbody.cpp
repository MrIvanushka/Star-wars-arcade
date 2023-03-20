#include "rigidbody.h"

#include <../../glm/glm/gtc/quaternion.hpp>
#include<iostream>
/*
    transformation functions
*/
#include <iostream>
// update position with velocity and acceleration
void RigidBody::update(float dt) {
    gameObject->move(velocity * dt + 0.5f * acceleration * (dt * dt));
    velocity += acceleration * dt;
    lastCollisionTime += dt;
}

// apply a force
void RigidBody::applyForce(glm::vec3 force) {
    acceleration += force / mass;
}

// apply a force
void RigidBody::applyForce(glm::vec3 direction, float magnitude) {
    applyForce(direction * magnitude);
}

// apply an acceleration (remove redundancy of dividing by mass)
void RigidBody::applyAcceleration(glm::vec3 a) {
    acceleration += a;
}

// apply an acceleration (remove redundancy of dividing by mass)
void RigidBody::applyAcceleration(glm::vec3 direction, float magnitude) {
    applyAcceleration(direction * magnitude);
}

// apply force over time
void RigidBody::applyImpulse(glm::vec3 force, float dt) {
    velocity += force / mass * dt;
}

// apply force over time
void RigidBody::applyImpulse(glm::vec3 direction, float magnitude, float dt) {
    applyImpulse(direction * magnitude, dt);
}

// transfer potential or kinetic energy from another object
void RigidBody::transferEnergy(float joules, glm::vec3 direction) {
    if (joules == 0) {
        return;
    }

    // comes from formula: KE = 1/2 * m * v^2
    glm::vec3 deltaV = direction * (float)sqrt(2 * abs(joules) / mass);

    velocity += joules > 0 ? deltaV : -deltaV;
}

/*
    collisions
*/
void RigidBody::handle(Collision collision) {
    if (lastCollisionTime >= COLLISION_THRESHOLD) {
        //gameObject->setActive(false);
        this->acceleration = glm::vec3(0.0f);
        this->velocity = glm::vec3(0.0f);
        //this->velocity = glm::reflect(this->velocity, glm::normalize(collision.Norm)); // register (elastic) collision
        lastCollisionTime = 0.0f; // reset counter
    }
}