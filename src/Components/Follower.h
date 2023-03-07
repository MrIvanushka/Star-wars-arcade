//
// Created by ivan on 14.04.2022.
//

#ifndef SWTOR_FOLLOWER_H
#define SWTOR_FOLLOWER_H

#include "../Engine/GameObject.h"

class Follower : public Component
{
private:
    OrientedPoint* parent;
    glm::vec3 deltaPos;
public:
    Follower(GameObject *object) : Component(object) {

    }

    void initialize(GameObject* parent, glm::vec3 deltaPos)
    {
        this->parent = parent;
        this->deltaPos = deltaPos;
    }

    void update(float deltaTime) {
        this->gameObject->rotateAt(this->parent->getRotation());
        this->gameObject->moveAt(this->parent->getPosition() - deltaPos * this->parent->getRotation());

    }
};

#endif //SWTOR_FOLLOWER_H
