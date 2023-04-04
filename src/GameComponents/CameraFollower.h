#ifndef SWTOR_CAMERAFOLLOWER_H
#define SWTOR_CAMERAFOLLOWER_H

#include"../../glm/glm/gtx/norm.hpp"

#include "../Engine/GameObject.h"

#include <cmath>
#include <iostream>


class CameraFollower : public Component
{
private:
    const float _minDistance = 1;
    const float _maxDistance = 10;
    const float _minVerticalAngle = 0;
    const float _maxVerticalAngle = 60;
    const float _rotatingSpeed = 10;

    OrientedPoint* _target;
    float _offset = 15;
    glm::vec2 _currentAngles;
    glm::vec2 _targetAngles;
public:
    CameraFollower(GameObject* object) : Component(object)
    {
        _currentAngles = glm::vec2(0.f);
        _targetAngles = glm::vec2(0.f);
    }

    void setTarget(OrientedPoint* target)
    {
        _target = target;
    }

    void update(float deltaTime) override{ 
        auto delta = _targetAngles - _currentAngles;

        if(glm::length(delta) > 0.00001f)
        {
        auto movingDelta = glm::normalize(delta) * _rotatingSpeed * deltaTime;

        if(glm::length(delta) < glm::length(movingDelta))
            movingDelta = delta;
        
        _currentAngles += movingDelta;
        }

        glm::quat localRotation;
        glm::quat parentRotation = _target->getRotation();
        glm::vec3 localOffset;

        localOffset.x = -1 * _offset * cos(_currentAngles.y) * cos(_currentAngles.x);
        localOffset.y = _offset * sin(_currentAngles.y);
        localOffset.z = -1 * _offset * cos(_currentAngles.y) * sin(_currentAngles.x);

        glm::quat rotY = glm::angleAxis(_currentAngles.x, glm::vec3(0,1,0));
        glm::quat rotX = glm::angleAxis(_currentAngles.y, glm::vec3(1,0,0));
        localRotation = rotX * rotY;

        gameObject->moveAt(_target->getPosition() + parentRotation * localOffset);
        gameObject->rotateAt(parentRotation * localRotation);
    }
    
    void rotate(glm::vec2 delta)
    {
        _targetAngles += delta;

        if (std::abs(_targetAngles.x) > 360) {
            if (_targetAngles.x < 0) {
                _targetAngles.x = std::fmod(_targetAngles.x, 360) + 360;
            } else {
                _targetAngles.x = std::fmod(_targetAngles.x, 360);
            }
        }

        if (_targetAngles.y > _maxVerticalAngle) _targetAngles.y = _maxVerticalAngle;
        if (_targetAngles.y < _minVerticalAngle) _targetAngles.y = _minVerticalAngle;

        //TODO: clamp angles
    }
};

#endif //SWTOR_CAMERAFOLLOWER_H