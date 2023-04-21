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
        std::vector<GameObject*> objects;
        glm::vec3 JediResp;
        glm::vec3 SithResp;

    public:
        Respawn(GameObject* obj) : Component(obj) {}

        void setRespawnPoints(glm::vec3 _jediResp, glm::vec3 _sithResp) {
            JediResp = _jediResp;
            SithResp = _sithResp;
        }
        
        void track(GameObject* obj) {
            objects.push_back(obj);
        }

        void update(float deltaTime) override {
            for (GameObject* obj : objects) {
                if (!obj->getComponent<HealthPresenter>()->isAlive()) {
                    switch (obj/*->getGameObject()*/->getComponent<FractionMember>()->getFraction())
                    {
                        case Fraction::Jedi:
                            obj/*->getGameObject()*/->move(JediResp - obj/*->getGameObject()*/->getPosition());
                            //obj->getComponent<HealthPresenter>()->heal(100);
                            break;
                        
                        case Fraction::Sith:
                            obj/*->getGameObject()*/->move(SithResp - obj/*->getGameObject()*/->getPosition());
                            //obj->getComponent<HealthPresenter>()->heal(100);
                            break;

                        default:
                            break;
                    }
                }
            }
        }
};

#endif //RESPAWM_H
