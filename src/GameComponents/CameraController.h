#ifndef SWTOR_CAMERACONTROLLER_H
#define SWTOR_CAMERACONTROLLER_H

#include "../Engine/GameObject.h"
#include "../Engine/Input.h"
#include "CameraFollower.h"

class CameraController : public Component
{
private:
    float _sensitivityX = 1;
    float _sensitivityY = 1;
    glm::vec2 _previousMousePosition;
    CameraFollower* _follower;
public:
    CameraController(GameObject* object) : Component(object)
    {}

    void start() override
    {
        _follower = gameObject->getComponent<CameraFollower>();
       // _previousMousePosition = Input::getInstance()->getMousePosition();
    }

    void update(float deltaTime) override
    {
        auto mousePosition = Input::getInstance()->getMousePosition();
        auto delta = mousePosition - _previousMousePosition;

        _follower->rotate(glm::vec2(delta.x * _sensitivityX, delta.y * _sensitivityY));

        //_previousMousePosition = mousePosition;
    }
    
};

#endif //SWTOR_CAMERACONTROLLER_H