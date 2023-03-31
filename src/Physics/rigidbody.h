#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <../../glm/glm/glm.hpp>
#include <string>
#include"../Utilities/Observer.h"
#include"../Engine/GameObject.h"
#include"Collider.h"

#define COLLISION_THRESHOLD 0.05f

/*
    Rigid Body class
    - represents physical body and holds all parameters
*/

class RigidBody : public Component, public ArgObserver<Collision> {
private:
    // mass in kg
    float mass;

    // velocity in m/s
    glm::vec3 velocity;
    // acceleration in m/s^2
    glm::vec3 acceleration;

    // rotation in Euler angles
    glm::vec3 rot;

    float lastCollisionTime = 0;
public:
    RigidBody(GameObject* object) : Component(object), ArgObserver<Collision>(), mass(1), velocity(0.0f), acceleration(0.0f), rot(0.0f)
    {}

    ~RigidBody() = default;
    /*
        transformation functions
    */

    // update position with velocity and acceleration
    void update(float dt);

    // apply a force
    void applyForce(glm::vec3 force);
    void applyForce(glm::vec3 direction, float magnitude);

    // apply an acceleration (remove redundancy of dividing by mass)
    void applyAcceleration(glm::vec3 acceleration);
    void applyAcceleration(glm::vec3 direction, float magnitude);

    // apply force over time
    void applyImpulse(glm::vec3 force, float dt);
    void applyImpulse(glm::vec3 direction, float magnitude, float dt);

    // transfer potential or kinetic energy from another object
    void transferEnergy(float joules, glm::vec3 direction);

    /*
        collisions
    */
    void handle(Collision collision) override;
};

#endif