//
// Created by egorich on 16.04.22.
//

#ifndef SWTOR_COLLISIONDETECTOR_H
#define SWTOR_COLLISIONDETECTOR_H

#include <utility>

#include "vector"
#include "../Components/Collider.h"
#include "../Components/SphereCollider.h"

class CollisionDetector {
public:
    CollisionDetector() = default;
    void initialize(const std::vector<GameObject*>& Bodies){
        for (auto Body : Bodies){
            Colliders.push_back(Body->getComponent<Collider>());
        }
    }
    void update();
private:
    std::vector<Collider*> Colliders = {};
    void CollisionProcessing(Collider* first, Collider* second);
    float Distance(Collider* first, Collider* second);
};


#endif //SWTOR_COLLISIONDETECTOR_H
