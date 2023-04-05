#ifndef SWTOR_CHARACTERCONTROLLER_H
#define SWTOR_CHARACTERCONTROLLER_H

#include "../Engine/GameObject.h"


class CharacterController : public Component
{
    float _speed = 3;
public:
    CharacterController(GameObject* object) : Component(object)
    {}

    void move(glm::vec3 direction, float deltaTime)
    {
        gameObject->move(direction * _speed * deltaTime);
        gameObject->rotateAt(glm::quatLookAt(direction, glm::vec3(0, 1, 0)));
    }

    void moveLocal(glm::vec3 direction, float deltaTime)
    {
        move(direction * gameObject->getRotation(), deltaTime);
    }
    
};

#endif //SWTOR_CHARACTERCONTROLLER_H
