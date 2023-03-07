//
// Created by egorich on 17.04.22.
//

#ifndef SWTOR_SPHERECOLLIDER_H
#define SWTOR_SPHERECOLLIDER_H

#include "vector"
#include "Collider.h"

class SphereCollider : public Collider{
private:
    float radius = 10;
public:
    float getRadius() const{return radius;}

    explicit SphereCollider(GameObject* object) : Collider(object) {}

    void initialize(float radius_){
        radius = radius_;
    }

    void CollisionExecution(Collider* aim) override{
        std::cout << "Collision from Sphere\n";
    }

};

#endif //SWTOR_SPHERECOLLIDER_H
