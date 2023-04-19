#ifndef SWTOR_BULLET_H
#define SWTOR_BULLET_H

#include"GameObject.h"




class Bullet : public Component{
private:
    float _speed;
    float _lifetime;
    float _flytime;
    float _damage;
    

    glm::vec3 _shootingDirection;
public:
    Bullet(GameObject* object) : Component(object){}

    void initialize(glm::vec3 shootingPos, glm::vec3 direction);

    void update(float deltaTime) override;
protected:
    void setSpeed(float speed);
    void setLifetime(float lifetime);
    void setDamage(float damage);
private:
    void upFlytime(float deltaTime);
};






#endif //SWTOR_BULLET_H

