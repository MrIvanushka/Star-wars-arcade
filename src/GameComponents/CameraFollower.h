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
    const float _rotatingSpeed = 100;

    OrientedPoint* _target;
    float _offset = 30;
    float _followingHeight = 10;
    glm::vec2 _currentAngles;
    glm::vec2 _targetAngles;
    
    glm::quat _targetRotation;
    glm::vec3 _currentLocalOffset;
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

        localOffset.x = _offset * cos(_currentAngles.y) * cos(_currentAngles.x);
        localOffset.y = _offset * sin(_currentAngles.y);
        localOffset.z = _offset * cos(_currentAngles.y) * sin(_currentAngles.x);

        glm::quat cameraRot = glm::angleAxis(glm::radians(90.f), glm::vec3(0,1,0));

        glm::quat rotY = glm::angleAxis(_currentAngles.x, glm::vec3(0,1,0));
        glm::quat rotX = glm::angleAxis(-1 * _currentAngles.y, glm::vec3(1,0,0));
        localRotation = rotX * cameraRot * rotY; 
        glm::vec3 globalOffset = localOffset;

        gameObject->moveAt(_target->getPosition() + globalOffset + glm::vec3(0,_followingHeight,0));
        gameObject->rotateAt(localRotation);
    }
    
    void rotate(glm::vec2 delta)
    {
        _targetAngles += delta * (-1.f);

        clampAngles(_targetAngles);
    }
private:
    void clampAngles(glm::vec2 &angles)
    {
        if (angles.y > glm::radians(_maxVerticalAngle)) angles.y = glm::radians(_maxVerticalAngle);
        if (angles.y < glm::radians(_minVerticalAngle)) angles.y = glm::radians(_minVerticalAngle);
    }
};

#endif //SWTOR_CAMERAFOLLOWER_H