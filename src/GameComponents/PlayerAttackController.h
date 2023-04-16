#ifndef SWTOR_PLAYERATTACKCONTROLLER_H
#define SWTOR_PLAYERATTACKCONTROLLER_H

#include "../Utilities/Observer.h"
#include "../Engine/Input.h"

class PlayerAttackController : public Component, public Observable
{
public:
    PlayerAttackController(GameObject* object) : Component(object) {}

    void update(float deltaTime) override{
        Input* input = Input::getInstance();

        if(input->keyIsPressed("LM"))
            invoke();
    }
};

#endif //SWTOR_PLAYERATTACKCONTROLLER_H