#ifndef COLLIDER_H
#define COLLIDER_H

#include"../Utilities/Observer.h"
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
public:
    bool isStatic = false;

	friend class Octree::node;
	friend class Ray;

    Collider(GameObject* object) : Component(object)
    {}

    ~Collider() = default;
    
    bool moved()
    {
	    return gameObject->isMoved();
    }

    OrientedPoint* getCenter()
    {
        return gameObject;
    }
private:
    void handleCollision(BoundingRegion br, glm::vec3 norm)
    {   
	    invoke(Collision(br, norm));
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