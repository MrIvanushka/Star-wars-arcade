#ifndef RESPAWM_H
#define RESPAWM_H

#include <vector>
#include "../Engine/GameObject.h"
#include "../GameComponents/IDamageable.h"
#include "../GameComponents/HealthPresenter.h"
#include "../GameComponents/FractionMember.h"

class Respawn : public Component
{
    private:
        std::vector<HealthPresenter*> objects;
        glm::vec3 Resp;
        Fraction fraction;

    public:
        Respawn(GameObject* obj, Fraction _fraction) : Component(obj), fraction(_fraction) {}

        void setRespawnPoint(glm::vec3 _Resp) {
            Resp = _Resp;
        }
        
        void track(HealthPresenter* obj) {
            if (this->fraction == obj->getGameObject()->getComponent<FractionMember>()->getFraction()) {
                objects.push_back(obj);
                return;
            }
            throw std::invalid_argument("Object's fraction must correspond to the respawn's fraction");
        }

        void update(float deltaTime) override {
            for (HealthPresenter* obj : objects) {
                if (!obj->isAlive()) {
                    obj->getGameObject()->moveAt(Resp);
                    obj->respawn();
                }
            }
        }
};

class JediRespawn : public Respawn
{
    public:
        JediRespawn(GameObject* obj) : Respawn(obj, Fraction::Jedi) {}
};

class SithRespawn : public Respawn
{
    public:
        SithRespawn(GameObject* obj) : Respawn(obj, Fraction::Sith) {}
};

#endif //RESPAWM_H
