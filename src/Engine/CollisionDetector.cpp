//
// Created by egorich on 16.04.22.
//

#include "CollisionDetector.h"
#include "../../glm/glm/gtx/intersect.hpp"


// Method returns the distance between the center of a SphereCollider and the closest point of collision model
// dist < radius => collision
float CollisionDetector::Distance(Collider *first, Collider *second) {
    auto* Sphere1 = dynamic_cast<SphereCollider*>(first);
    auto* Sphere2 = dynamic_cast<SphereCollider*>(second);
    //auto* Mesh1 = dynamic_cast<MeshCollider*>(first);
    //auto* Mesh2 = dynamic_cast<MeshCollider*>(second);

    // ==========Sphere1 and Sphere2==========
    if (Sphere1 != nullptr and Sphere2 != nullptr){
        return glm::length(Sphere1->getObject()->getPosition() - Sphere2->getObject()->getPosition());
    }
}

void CollisionDetector::CollisionProcessing(Collider* first, Collider* second){
    if (first != nullptr and second != nullptr){

        float distance = Distance(first, second);
        auto* Sphere1 = dynamic_cast<SphereCollider*>(first);
        auto* Sphere2 = dynamic_cast<SphereCollider*>(second);

        if (Sphere1 != nullptr and Sphere2 != nullptr){
            if (distance < Sphere1->getRadius() + Sphere2->getRadius()){
                Sphere1->CollisionExecution(Sphere2);
                Sphere2->CollisionExecution(Sphere1);
            }
        }

        else if (Sphere1 != nullptr){
            if (distance < Sphere1->getRadius()){
                Sphere1->CollisionExecution(second);
                second->CollisionExecution(Sphere1);
            }
        }

        else if (Sphere2 != nullptr){
            if (distance < Sphere2->getRadius() and std::abs(distance - 0.237693) >= 1E-3){
                Sphere2->CollisionExecution(first);
                first->CollisionExecution(Sphere2);
            }
        }
    }
}


void CollisionDetector::update(){
    for (int i = 0; i < Colliders.size(); ++i){
        for (int j = i + 1; j < Colliders.size(); ++j){
            if (Colliders[j] != Colliders[i]){

                CollisionProcessing(Colliders[i], Colliders[j]);

            }
        }
    }
}


