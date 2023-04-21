#include "Bullet.h"
#include "IDamageable.h"

void Bullet::initialize(glm::vec3 shootingPos, glm::vec3 direction){
    direction = glm::normalize(direction);

    gameObject->moveAt(shootingPos);
    gameObject->rotateAt(glm::quatLookAt(direction, glm::vec3(0,1,0)));
    _shootingDirection = direction;
    gameObject->setActive(true);
    _flytime = 0;
}

void Bullet::start(){
    gameObject->getComponent<Collider>()->Attach(this);
}

void Bullet::update(float deltaTime){
    gameObject->move(_shootingDirection * _speed * deltaTime);
    upFlytime(deltaTime);
}

void Bullet::handle(Collision collision){
    IDamageable* enemyHealth = collision.Br.collider->getComponent<IDamageable>();

    if(_lifetime > 0.1f && gameObject->activeSelf() &&  !collision.Br.collider->isTrigger())
    {
    if(enemyHealth != nullptr)
        enemyHealth->takeDamage(_damage);

    gameObject->setActive(false);
    }
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
