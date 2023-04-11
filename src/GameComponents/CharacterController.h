#ifndef SWTOR_CHARACTERCONTROLLER_H
#define SWTOR_CHARACTERCONTROLLER_H

#include "../Engine/GameObject.h"


class CharacterController : public Component
{
    float _maxSpeed = 10;
    float _acceleration = 40;
    glm::quat _offsetRotation;

    glm::vec3 _velocity;

public:
    CharacterController(GameObject* object) : Component(object)
    {
        _offsetRotation = glm::angleAxis(glm::radians(180.f), glm::vec3(0,1,0));
        _velocity = glm::vec3(0.f);
        
    }

    void update(float deltaTime) override{
        gameObject->move(_velocity * deltaTime);

        if(glm::length(_velocity) > 0.001f)
        {
            glm::vec3 movingDirection = glm::normalize(_velocity);
            gameObject->rotateAt(_offsetRotation * glm::quatLookAt(movingDirection, glm::vec3(0, 1, 0)));
        }
    }

    void accelerate(glm::vec3 direction, float deltaTime)
    {
        _velocity += direction * _acceleration * deltaTime;
        float currentSpeed = glm::length(_velocity);

        if(currentSpeed > _maxSpeed)
            _velocity *= _maxSpeed / currentSpeed;
    }

    void slowDown(float deltaTime)
    {
        float currentSpeed = glm::length(_velocity);
        float delta = _acceleration * deltaTime;

        if(currentSpeed <= delta)
            _velocity = glm::vec3(0.f);
        else
            _velocity *= (currentSpeed - delta) / currentSpeed;
    }

    float getCurrentSpeed()
    {
        return glm::length(_velocity);
    }

    float getMaxSpeed()
    {
        return _maxSpeed;
    }
};

#endif //SWTOR_CHARACTERCONTROLLER_H
