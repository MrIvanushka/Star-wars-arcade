#include "Blaster.h"



void Blaster::setMuzzlePosition(glm::vec3 muzzle_position){
    this->muzzle_position = muzzle_position;
}
void Blaster::setRotation( glm::quat rotation){
    this->rotation = rotation;
}
void Blaster::setRotation(glm::vec3 rotation){
    this->rotation = glm::quat(rotation * (3.1415f / 180.f));
}




void Blaster::setCooldown(float cooldown){
    this->cooldown = cooldown;
}
void Blaster::setLimitOverheating(float limit_overheating){
    this->limit_overheating = limit_overheating;
}
void Blaster::setHeatingPerShot(float heating_per_shot){
    this->heating_per_shot = heating_per_shot;
}
void Blaster::setCoolingPerTime(float cooling_per_time){
    this->cooling_per_time = cooling_per_time;

}
void Blaster::Heating(){
    if(this->overheating + heating_per_shot < this->limit_overheating){
        this->overheating += heating_per_shot;
    }
    else{
        this->overheating = this->limit_overheating;
    }
}
void Blaster::Cooling(){
    if(this->overheating - cooling_per_time > 0){
        this->overheating -= cooling_per_time;
    }
    else{
        this->overheating = 0;

    }
}


GameObject Bullet_obj();

void Blaster::initialize(std::queue<Bullet*> queue_of_bullets)
{
    this->queue_of_bullets = queue_of_bullets;
}

void Blaster::shoot(){
    if(not Switch and overheating == 0){
        Switch = true;
    }
    if(Switch){
        Bullet* bullet = queue_of_bullets.front();
        bullet->initialize(muzzle_position, this->rotation * gameObject->getRotation());
        queue_of_bullets.pop();
        queue_of_bullets.push(bullet);

        Heating();

        if(overheating == limit_overheating){
            Switch = false;
        }
    }
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




