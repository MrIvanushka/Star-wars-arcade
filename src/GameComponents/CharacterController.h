#ifndef SWTOR_CHARACTERCONTROLLER_H
#define SWTOR_CHARACTERCONTROLLER_H

#include "../Engine/GameObject.h"


class CharacterController : public Component
{

public:
    CharacterController(GameObject* object) : Component(object)
    {}

    void move(glm::vec3 direction);
    
};

#endif //SWTOR_CHARACTERCONTROLLER_H
