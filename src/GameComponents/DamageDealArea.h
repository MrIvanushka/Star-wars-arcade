#ifndef SWTOR_DAMAGEDEALAREA_H
#define SWTOR_DAMAGEDEALAREA_H

#include "../Engine/GameObject.h"
#include "../Physics/Collider.h"
#include "IDamageable.h"
#include "FractionMember.h"

class DamageDealArea : public Component, public Observer
{
private:
    Collider* _selfCollider;
    Fraction _selfFraction;
    float _damage = 200;
public:
    DamageDealArea(GameObject* object) : Component(object) {}

    void start() override{
        _selfCollider = gameObject->getComponent<Collider>();
    }

    void initialize(Fraction selfFraction){
        _selfFraction = selfFraction;
    }

    void handle() override{
        dealDamage();
    }

    void dealDamage(){
        auto touchedColliders = _selfCollider->getTouchedColliders();
        
        for(auto touchedCollider : touchedColliders){
            IDamageable* damageableObject = touchedCollider->getComponent<IDamageable>();
            FractionMember* member = touchedCollider->getComponent<FractionMember>();

            if(damageableObject != nullptr && member != nullptr)
                if(member->getFraction() != _selfFraction)
                    damageableObject->takeDamage(_damage);
        }
    }
};

#endif //SWTOR_DAMAGEDEALAREA_H