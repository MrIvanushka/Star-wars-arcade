#include "Blaster.h"

void Blaster::setLimitOverheating(float limit_overheating){
    _limit_overheating = limit_overheating;
}
void Blaster::setHeatingPerShot(float heating_per_shot){
    _heating_per_shot = heating_per_shot;
}
void Blaster::setCoolingPerTime(float cooling_per_time){
    _cooling_per_time = cooling_per_time;
}
void Blaster::setReloadingTime(float reloadingTime){
    _reloadingTime = reloadingTime;
}

void Blaster::Heating(){
    if(_overheating + _heating_per_shot < _limit_overheating){
        _overheating += _heating_per_shot;
    }
    else{
        _overheating = _limit_overheating;
    }
}
void Blaster::Cooling(float deltaTime){
    float coolValue = _cooling_per_time * deltaTime;

    if(_overheating - coolValue > 0){
        _overheating -= coolValue;
    }
    else{
        _overheating = 0;

    }
}

void Blaster::initialize(std::queue<Bullet*> queue_of_bullets)
{
    this->queue_of_bullets = queue_of_bullets;
}

void Blaster::shoot(glm::vec3 targetPos){

    if(!_notOverheated && _overheating < 0.01){
        _notOverheated = true;
    }
    if(canShoot()){
        Bullet* bullet = queue_of_bullets.front();
        bullet->initialize(gameObject->getPosition(), targetPos - gameObject->getPosition());
        queue_of_bullets.pop();
        queue_of_bullets.push(bullet);
        _currentTime = 0;

        Heating(); 

        if(_overheating >= _limit_overheating){
            _notOverheated = false;
        }
    }
}

void Blaster::update(float deltaTime){
    _currentTime += deltaTime;
    Cooling(deltaTime);
}

float Blaster::getLimitOverheating(){
    return _limit_overheating;
}
float Blaster::getHeatingPerShot(){
    return _heating_per_shot;
}
float Blaster::getCoolingPerTime(){
    return _cooling_per_time;
}
bool Blaster::canShoot() {
    return (_notOverheated && _currentTime > _reloadingTime);
}



