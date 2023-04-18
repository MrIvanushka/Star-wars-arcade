#ifndef SWTOR_HEALTHPRESENTER_H
#define SWTOR_HEALTHPRESENTER_H

#include "../Engine/GameObject.h"
#include "CharacterHealth.h"
#include "IDamageable.h"

class HealthPresenter : public Component, public HealthAttribute
{
public:
    HealthPresenter(GameObject* object) : Component(object), HealthAttribute(new CharacterHealth(100)) {}

    void update(float deltaTime) override{
        if(isAlive() == false)
            gameObject->setActive(false);
    }

    template<typename Attribute>
    void boost(){
        Attribute* newAttribute = new Attribute(_health);
        _health = newAttribute;
    }
};