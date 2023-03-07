//
// Created by egorich on 17.04.22.
//

#ifndef SWTOR_COLLIDER_H
#define SWTOR_COLLIDER_H

#include<iostream>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include "../../glm/glm/glm.hpp"
#include "../../glm/glm/vec3.hpp"
#include<../../glm/glm/mat4x4.hpp>
#include<../../glm/glm/gtc/matrix_transform.hpp>
#include<../../glm/glm/gtx/euler_angles.hpp>

#include "../Engine/GameObject.h"

class Collider : public Component {
private:

protected:
    std::set<Collider*> CollidedWith;
    bool IsCollided = false;
public:
    void insertCollidedWith(Collider* collider){
        CollidedWith.insert(collider);
    }
    void setIsCollided(bool is_collided){ IsCollided = is_collided; }
    Collider(GameObject *gameObject) : Component(gameObject, true) {}
    GameObject* getObject(){
        return gameObject;
    }
    virtual void CollisionExecution(Collider* aim){}
};

#endif //SWTOR_COLLIDER_H
