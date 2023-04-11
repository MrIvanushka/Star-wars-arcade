#ifndef SWTOR_PARTICLE_H
#define SWTOR_PARTICLE_H

#include "../Engine/GameObject.h"
#include "../BaseComponents/Camera.h"

class Particle : public Component {
private:
    float _lifetime = 0;
    float _time = 0;
    glm::vec3 _speed;
    glm::vec3 _startingPosition;
    IReadonlyCamera* _camera;  
    ParticleSystem* _parent;

public:
    Particle(GameObject* object) : Component(object) 
    {
        _speed = glm::vec3(0.f);
        _startingPosition = glm::vec3(0.f);
    }

    void setParent(ParticleSystem* system){
        _parent = system;
    }

    void activate(float lifetime, glm::vec3 speed, glm::vec3 startingPosition, Camera& camera) {
        _lifetime = lifetime;
        _speed = speed;
        _startingPosition = startingPosition;
        _camera = &camera;
        gameObject->setActive(true);
    }

    float returnHealth() {
        if (_lifetime) return _time / _lifetime;
        else return 100;
    }

    void setVelocity() {}

    void update(float delTatime) override {
        _parent->updateParticle(this);
        _time += delTatime;
        if (_time >= _lifetime) {
            _time = _lifetime;
            gameObject->setActive(false);
        }
        gameObject->move(delTatime * _speed);
        gameObject->rotateAt(glm::quatLookAt(_camera.getFront(), glm::vec3(0,1,0))); // Billboard
    }
};

#endif //SWTOR_PARTICLE_H