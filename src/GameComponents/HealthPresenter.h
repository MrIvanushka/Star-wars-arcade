#ifndef SWTOR_HEALTHPRESENTER_H
#define SWTOR_HEALTHPRESENTER_H

#include "../Engine/GameObject.h"
#include "CharacterHealth.h"
#include "IDamageable.h"
#include "FractionMember.h"

class HealthPresenter : public Component, public HealthAttribute, public FractionMember
{
public:
    HealthPresenter(GameObject* object) : Component(object), HealthAttribute(new CharacterHealth(100)) {}

    void update(float deltaTime) override{
        if(isAlive() == false)
            gameObject->setActive(false);
    }

    void setFraction(Fraction newFraction){
        changeFraction(newFraction);
    }

    void respawn() override{
        this->getNextAttribute()->respawn();
        gameObject->setActive(true);
    }
/*
    template<typename Attribute>
    void boost(){
        Attribute* newAttribute = new Attribute(_health);
        _health = newAttribute;
    }*/
};

#endif