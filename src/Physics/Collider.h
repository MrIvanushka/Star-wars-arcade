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
private:
    std::set<Collider*> _touchedColliders;
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

    OrientedPoint* getCenter()
    {
        return gameObject;
    }

    void update(float deltaTime) override
    {
        std::set<Collider*> _previousTouchedColliders(_touchedColliders);

        if(moved())
            _touchedColliders = std::set<Collider*>();
        else
            for(auto touchedCollider : _previousTouchedColliders)
                if(touchedCollider->moved() == false)
                    _touchedColliders.insert(touchedCollider);

    }
private:
    void handleCollision(BoundingRegion br, glm::vec3 norm)
    {   
        if(_touchedColliders.find(br.collider) != _touchedColliders.end())
            invoke(Collision(br, norm));

        _touchedColliders.insert(br.collider);
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