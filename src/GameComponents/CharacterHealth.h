#ifndef SWTOR_CHARACTERHEALTH_H
#define SWTOR_CHARACTERHEALTH_H

#include "IDamageable.h"

class CharacterHealth : public IDamageable {
private:
    float _health;
    const float _maxHealth;
public:
    CharacterHealth(float health): _health(health), _maxHealth(health) {}
    void takeDamage(float damage) override;
    void heal(float healValue) override;
    float getMaxHealth() override;
    float getHealth() override;
    bool isAlive() override;
    void respawn() override;
};

#endif