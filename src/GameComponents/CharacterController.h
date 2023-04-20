#ifndef SWTOR_CHARACTERCONTROLLER_H
#define SWTOR_CHARACTERCONTROLLER_H

#include<vector>

#include "../Engine/GameObject.h"
#include "../Physics/meshcollider.h"
//#include "../Physics/TerrainCollider.h"

class CharacterController : public Component, public ArgObserver<Collision>
{
    const float _maxGroundAngle = 45;
    const float _maxSpeed = 25;
    const float _acceleration = 60;
    const float _jumpForce = 1000;
    const glm::vec3 _gravityAcceleration = glm::vec3(0, -50.f, 0);
    glm::quat _offsetRotation;

    glm::vec3 _velocity;
    glm::vec3 _gravityVelocity;
    glm::vec3 _groundPos;

    bool _isGrounded = true;

    MeshCollider* _selfCollider;
    Collider* _stepTrigger = nullptr;
    std::set<Collider*> _touchedColliders;
    
    std::vector<glm::vec3> _groundNormals;
    std::vector<glm::vec3> _wallNormals;

public:

    CharacterController(GameObject* object) : Component(object) {}

    void addStepTrigger(Collider* collider);

    void start() override;

    void update(float deltaTime) override;

    void accelerate(glm::vec3 direction, float deltaTime);

    void slowDown(float deltaTime);

    void jump(float deltaTime);

    float getCurrentSpeed();

    float getGravitySpeed();

    float getMaxSpeed();

    bool isGrounded();

    void handle(Collision collision) override;

private:

    void move(float deltaTime);

    void fall(float deltaTime);

    void processNorm(Collider* touchedCollider, glm::vec3 norm, glm::vec3 touchPos);

    void processCollisions();
};

#endif //SWTOR_CHARACTERCONTROLLER_H
