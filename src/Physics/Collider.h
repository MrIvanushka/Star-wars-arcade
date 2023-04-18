#ifndef COLLIDER_H
#define COLLIDER_H

#include"../Utilities/Observer.h"
#include"../Engine/GameObject.h"
#include"bounds.h"
#include<iostream>

struct Collision
{
	BoundingRegion Br;
	glm::vec3 Norm;

	Collision(BoundingRegion br, glm::vec3 norm) : Br(br), Norm(norm)
	{}
};

class Collider : public Component, public ArgObservable<Collision> {
private:
    bool _isTrigger = false;
    std::set<Collider*> _touchedColliders;
    std::set<Collider*> _newColliders;
public:
	friend class Octree::node;
	friend class Ray;

    Collider(GameObject* object) : Component(object)
    {}

    ~Collider() = default;
    
    void setAsTrigger()
    {
        _isTrigger = true;
    }

    bool isTrigger(){
        return _isTrigger;
    }

    bool moved()
    {
	    return gameObject->isMoved();
    }

    GameObject* getCenter()
    {
        return gameObject;
    }

    void update(float deltaTime) override
    {
        if(moved())
        {
            _touchedColliders = _newColliders;
        }
        else
        {
            std::set<Collider*> _previousColliders(_touchedColliders);

            for(auto collider : _previousColliders)
                if(collider->moved())
                    _touchedColliders.erase(collider);

            for(auto collider : _newColliders)
                _touchedColliders.insert(collider);
        }

        _newColliders = std::set<Collider*>();
    }

    std::set<Collider*> getTouchedColliders()
    {
        return _touchedColliders;
    }

    bool touches(Collider* collider) 
    {
        return _touchedColliders.contains(collider);
    }
private:
    void handleCollision(BoundingRegion br, glm::vec3 norm)
    {   
        if(_touchedColliders.find(br.collider) != _touchedColliders.end())
            invoke(Collision(br, norm));

        _newColliders.insert(br.collider);
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