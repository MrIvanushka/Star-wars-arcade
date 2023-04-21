#include "Blaster.h"

void Blaster::setCooldown(float cooldown){
    _cooldown = cooldown;
}
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
    if(this->overheating + _heating_per_shot < this->limit_overheating){
        this->overheating += _heating_per_shot;
    }
    else{
        _overheating = _limit_overheating;
    }
}
void Blaster::Cooling(float deltaTime){
    float coolValue = _cooling_per_time * deltaTime;

    if(this->overheating - coolValue > 0){
        this->overheating -= coolValue;
    }
    else{
        this->overheating = 0;

    }
}

void Blaster::initialize(std::queue<Bullet*> queue_of_bullets)
{
    this->queue_of_bullets = queue_of_bullets;
}

void Blaster::shoot(glm::vec3 targetPos){
    if(!_notOverheated && _overheating == 0){
        _notOverheated = true;
    }
    if(_notOverheated){
        Bullet* bullet = queue_of_bullets.front();
        bullet->initialize(gameObject->getPosition, targetPos - gameObject->getPosition());
        queue_of_bullets.pop();
        queue_of_bullets.push(bullet);

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

glm::vec3 Blaster::getMuzzlePosition(){
    return(gameObject->getPosition + getRotation() * this->muzzle_position)_
}
glm::quat Blaster::getRotation(){
    return this->rotation;
}

float Blaster::getCooldown(){
    return this->cooldown;
}
float Blaster::getLimitOverheating(){
    return this->limit_overheating;
}
float Blaster::getHeatingPerShot(){
    return this->heating_per_shot;
}
float Blaster::getCoolingPerTime(){
    return this->cooling_per_time;
}
bool Blaster::canShoot() {
    return (_notOverheated && _currentTime > _reloadingTime);
}



