#ifndef COLLIDER_H
#define COLLIDER_H

#include"../Model/Observer.h"
#include"../Engine/GameObject.h"
#include"bounds.h"

struct Collision
{
	BoundingRegion Br;
	glm::vec3 Norm;

	Collision(BoundingRegion br, glm::vec3 norm) : Br(br), Norm(norm)
	{}
};

class Collider : public Component, public ArgObservable<Collision> {
private:
    void handleCollision(BoundingRegion br, glm::vec3 norm)
    {   
	    invoke(Collision(br, norm));
    }
public:
	friend class Octree::node;
	friend class Ray;

    Collider(GameObject* object) : Component(object)
    {}

    ~Collider() = default;
    
    bool moved()
    {
	    return gameObject->isMoved();
    }
};

class SphereCollider : public Collider
{
public:
    friend class Octree::node;
	friend class Ray;

    SphereCollider(GameObject* object) : Collider(object)
    {}

    ~SphereCollider() = default;
};

#endif