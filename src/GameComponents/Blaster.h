#ifndef SWTOR_BLASTER_H
#define SWTOR_BLASTER_H


#include <queue>

#include"../Engine/GameObject.h"
#include"Bullet.h"


class Blaster : public Component{
protected:
    float _overheating = 0;
    float _heating_per_shot = 1;
    float _limit_overheating = 5;
    float _cooling_per_time = 1;
    float _reloadingTime = 0.5;
    float _currentTime = 0;
    bool _notOverheated = true;
private:
    std::queue<Bullet*> queue_of_bullets;
public:
    Blaster(GameObject* object) : Component(object){}

    void setLimitOverheating(float limit_overheating);
    void setHeatingPerShot(float heating_per_shot);
    void setCoolingPerTime(float cooling_per_time);
    void setReloadingTime(float reloadingTime);

    void initialize(std::queue<Bullet*> queue_of_bullets);

    void update(float deltaTime) override;

    void shoot(glm::vec3 targetPos);

    float getLimitOverheating();
    float getHeatingPerShot();
    float getCoolingPerTime();
    bool canShoot();
private:
    void Heating();
    void Cooling(float deltaTime);
};


#endif //SWTOR_BLASTER_H