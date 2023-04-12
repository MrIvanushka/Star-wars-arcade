#include <iostream>
#include "Character.h"

bool Character::IsDead() {
    return (_health <= 0);
}

float Character::GetHealth() {
    return _health;
}

void Character::TakeDamage(float damage) {
    if (damage <= 0) throw std::logic_error("Damage can't be negative");
    if (_health <= 0) throw std::logic_error("Player is dead");
    _health -= damage;
}

void Character::Heal(float health) {
    if (health <= 0) throw std::logic_error("Healing can't be negative");
    if (_health <= 0) throw std::logic_error("Player is dead");
    _health += health;
    if (_health > _maxHealth) _health = _maxHealth;
}
