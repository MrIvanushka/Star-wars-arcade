#ifndef SWTOR_PARTICLESYSTEM_H
#define SWTOR_PARTICLESYSTEM_H

#include "../Engine/GameObject.h"
#include "../GameComponents/Particle.h"
#include "../BaseComponents/Camera.h"

class ParticleSystem : public Component 
{
private:
    std::queue<Particle*> _particles;
    glm::vec3 _particleVelocity;
    float _particleLifetime;
    IReadonlyCamera* _camera;  

public:
    ParticleSystem(GameObject* object) : Component(object)
    {}

    void setup(glm::vec3 particleVelocity, float particleLifetime, std::vector<GameObject> &sceneObjects, Camera& camera) {
        _particleVelocity = particleVelocity;
        _particleLifetime = particleLifetime;
        _camera = &camera;
        for (auto i = 0u; i < sceneObjects.size(); i++) {
            Particle particle(sceneObjects[i]);
            _particles.push(particle);
        }
    }

    virtual void updateParticle(Particle* particle) {}

protected:
    void spawnParticle(Particle* particle) {
        particle->activate(_particleLifetime, _particleVelocity, gameObject->getPosition(), _camera);
    }
private:
    virtual void initializeParticle(Particle* particle) {}
};

class ParticleSystemSplash : public ParticleSystem
{
public: 
    void onEnable() override {
        for (auto particle : _particles) {
            spawnParticle(particle);
        }
    }
};

class ParticleSystemDurational : public ParticleSystem
{
private:
    float _duration;
    float _gone = 0;

public:
    void setDuration(float duration) {
        _duration = duration;
    }

    void update(float deltaTime) override {
        if (_gone >= _duration) {
            auto particle = _particles[0];
            _particles.pop();
            spawnParticle(particle);
            _particles.push(particle);
            _gone = 0;
        }
        _gone += deltaTime;
    }
};

#endif //SWTOR_PARTICLESYSTEM_H