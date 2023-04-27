#include<stdexcept>
#include "CharacterHealth.h"

bool CharacterHealth::isAlive() {
    return (_health > 0);
}

float CharacterHealth::getMaxHealth() {
    return _maxHealth;
}


float CharacterHealth::getHealth() {
    return _health;
}

void CharacterHealth::takeDamage(float damage) {
    if (damage <= 0) throw std::logic_error("Damage can't be negative");
    if (_health <= 0) throw std::logic_error("Player is dead");
    _health -= damage;
}

void CharacterHealth::heal(float healValue) {
    if (healValue <= 0) throw std::logic_error("Healing can't be negative");
    //if (_health <= 0) throw std::logic_error("Player is dead");
    _health += healValue;
    if (_health > _maxHealth) _health = _maxHealth;
}