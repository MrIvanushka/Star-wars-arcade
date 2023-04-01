#ifndef SWTOR_CAMERAFOLLOWER_H
#define SWTOR_CAMERAFOLLOWER_H

#include"../../glm/glm/gtx/norm.hpp"

#include "../Engine/GameObject.h"


class CameraFollower : public Component
{
private:
    const float _minDistance = 1;
    const float _maxDistance = 10;
    const float _minVerticalAngle = 0;
    const float _maxVerticalAngle = 60;
    const float _rotatingSpeed = 10;

    OrientedPoint* _target;
    float _offset;
    glm::vec2 _currentAngles;
    glm::vec2 _targetAngles;
public:
    CameraFollower(GameObject* object) : Component(object)
    {}

    void setTarget(OrientedPoint* target)
    {
        _target = target;
    }

    void update(float deltaTime) override{ 
        auto delta = _targetAngles - _currentAngles;
        auto movingDelta = glm::normalize(delta) * _rotatingSpeed * deltaTime;

        if(glm::length(delta) < glm::length(movingDelta))
            movingDelta = delta;

        glm::quat localRotation;
        glm::quat parentRotation = _target->getRotation();
        glm::vec3 localOffset;
        //use sin and cos to get local camera coord
        //search function to get quatenion from forward vector
        gameObject.moveAt(_target->getPosition() + localOffset * parentRotation);
        gameObject.rotateAt(parentRotation * localRotation);
    }
    
    void rotate(glm::vec2 delta)
    {
        _targetAngles += delta;

        //TODO: clamp angles
    }
};

#endif //SWTOR_CAMERAFOLLOWER_H