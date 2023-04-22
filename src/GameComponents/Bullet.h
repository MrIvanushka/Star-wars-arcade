#ifndef SWTOR_BULLET_H
#define SWTOR_BULLET_H

#include"../Engine/GameObject.h"
#include"../Physics/Collider.h"

class Bullet : public Component, public ArgObserver<Collision> {
private:
    float _speed = 100;
    float _lifetime = 1;
    float _flytime = 0;
    float _damage = 200;
    
    glm::vec3 _shootingDirection;
public:
    Bullet(GameObject* object) : Component(object){}

    void initialize(glm::vec3 shootingPos, glm::vec3 direction);

    void start() override;

    void update(float deltaTime) override;

    void handle(Collision collision) override; 
protected:
    void setSpeed(float speed);
    void setLifetime(float lifetime);
    void setDamage(float damage);
private:
    void upFlytime(float deltaTime);
};






#endif //SWTOR_BULLET_H

