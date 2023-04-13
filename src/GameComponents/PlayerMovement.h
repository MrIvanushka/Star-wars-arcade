#ifndef SWTOR_PLAYERMOVEMENT_H
#define SWTOR_PLAYERMOVEMENT_H

#include "../Engine/GameObject.h"
#include "../Engine/Input.h"
#include"CharacterController.h"
#include"../BaseComponents/Camera.h"

class PlayerMovement : public Component
{
private:
    CharacterController* _controller;
    IReadonlyCamera* _camera;
public:
    PlayerMovement(GameObject* object) : Component(object)
    {}

    void start() override
    {
        _controller = gameObject->getComponent<CharacterController>();
    }

    void attachCamera(IReadonlyCamera* camera){
        _camera = camera;
    }

    void update(float deltaTime) override
    {
        glm::vec3 direction(0.f);
        Input* input = Input::getInstance();

        if(input->keyIsPressed("W"))
            direction += _camera->getFront();
        if(input->keyIsPressed("S"))
            direction -= _camera->getFront();
        if(input->keyIsPressed("D"))
            direction += _camera->getRight();
        if(input->keyIsPressed("A"))
            direction -= _camera->getRight();

        if(glm::length(direction) > 0.00001f)
        {
            direction.y = 0.f;

            _controller->accelerate(glm::normalize(direction), deltaTime);
        }
        else{
            _controller->slowDown(deltaTime);
        }

        if(input->keyIsPressed("Space"))
            _controller->jump(deltaTime);
    }
    
};

#endif //SWTOR_MOVEMENT_H