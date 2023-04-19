#ifndef SWTOR_FOLLOWER_H
#define SWTOR_FOLLOWER_H

#include "../Engine/GameObject.h"

class Follower : public Component
{
private:
    OrientedPoint* _target;
    
    glm::vec3 _positionOffset;
    glm::quat _rotationOffset;
public:
    Follower(GameObject* object) : Component(object) {}

    void setTarget(OrientedPoint* target)
    {
        _target = target;
    }

    void start() override{
        _positionOffset = gameObject->getPosition() - _target->getPosition();
        _rotationOffset = glm::inverse(_target->getRotation()) * gameObject->getRotation();
    }

    void update(float deltaTime) override{ 
        gameObject->moveAt(_positionOffset * glm::inverse(_target->getRotation()) + _target->getPosition());
        gameObject->rotateAt(_rotationOffset * _target->getRotation());
    }
};

#endif //SWTOR_FOLLOWER_H