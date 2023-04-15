#ifndef SWTOR_HEALTHPRESENTER_H
#define SWTOR_HEALTHPRESENTER_H

#include "../Engine/GameObject.h"
#include "IDamageable.h"

class HealthPresenter : public Component, public HealthAttribute
{
public:
    HealthPresenter(GameObject* object) : Component(object) {}

    void update(float deltaTime) override{
        _currentTime += deltaTime;
    }

    void boost(HealhAttribute* newAttribute){
        IDamageable* oldHealth = _health;
        _health = newAttribute;
        newAttribute->addDamageable(oldHealth);
    }

    void buff(TemporaryHealthAttribute* newAttribute){
        boost(newAttribute);
        newAttribute->initialize(&_health);
    }
};