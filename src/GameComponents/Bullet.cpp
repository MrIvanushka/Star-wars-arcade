#include "Bullet.h"

void initialize(glm::vec3 shootingPos, glm::vec3 direction){
    direction = glm::normalize(direction);

    gameObject->moveAt(shootingPos);
    gameObject->rotateAt(glm::quatLookAt(direction, glm::vec3(0,1,0)));
    _shootingDirection = direction;
    gameObject->setActive(true);
    _flytime = 0;
}

void Bullet::update(float deltaTime){
    gameObject->move(_shootingDirection * _speed * deltaTime);
    upFlytime(deltaTime);
}

void Bullet::setSpeed(float speed){
    this->_speed = speed;
}
void Bullet::setLifetime(float lifetime){
    this->_lifetime = lifetime;
}
void Bullet::setDamage(float damage){
    this->_damage = damage;
}




void Bullet::upFlytime(float deltaTime){
    if(_flytime < _lifetime){
        _flytime += deltaTime;
    }
    else{
        gameObject->setActive(false);
    }
}
