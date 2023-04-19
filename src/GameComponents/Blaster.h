#ifndef SWTOR_BLASTER_H
#define SWTOR_BLASTER_H


#include <queue>

#include"GameObject.h"
#include"Bullet.h"


class Blaster : public Component{
protected:
    glm::vec3 muzzle_position;
    glm::quat rotation;
    float cooldown;
    float overheating=0;
    float heating_per_shot;
    float limit_overheating;
    float cooling_per_time;
    bool Switch = true;
public:
    Blaster(GameObject* object) : Component(object){}

    void setMuzzlePosition(glm::vec3 muzzle_position);
    void setRotation(glm::quat rotation);
    void setRotation(glm::vec3 rotation);
    void setCooldown(float cooldown);
    void setLimitOverheating(float limit_overheating);
    void setHeatingPerShot(float heating_per_shot);
    void setCoolingPerTime(float cooling_per_time);

    void initialize(std::queue<Bullet*> queue_of_bullets);



    // Стрельба
    void shoot();


    glm::vec3 getMuzzlePosition();
    glm::quat getRotation();
    float getCooldown();
    float getLimitOverheating();
    float getHeatingPerShot();
    float getCoolingPerTime();

private:
    std::queue<Bullet*> queue_of_bullets;
    void Heating();
    void Cooling();
}


/*
Создать класс пули, компонент. Пуля имеет скорость и направление.
При выстреле активируется(gameObject->setActive),
получает направление и точку выстрела. Пуля имеет время жизни.

Создать класс бластера (тоже компонент).
У бластера есть локальные координаты дула,
публичный метод для выстрела. Бластер имеет скорость перезарядки.
В бластере лежит очередь из пуль (см. std::queue).
*/


#endif //SWTOR_BLASTER_H