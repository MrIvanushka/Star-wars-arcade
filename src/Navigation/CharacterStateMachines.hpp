#ifndef CHARACTERSTATEMACHINES_H
#define CHARACTERSTATEMACHINES_H

#include "../Engine/GameObject.h"
#include "../Physics/Collider.h"

#include "CharacterStates.hpp"

class AI : public Component 
{
    protected:
        StateMachine* _stateMachine;
    
    private:
        Vision* m_vision;

    public:
        AI(GameObject* obj, Vision* vision) : Component(obj), m_vision(vision) {}

        void update(float deltaTime) override {
            _stateMachine->update(deltaTime);
        }

}

#endif
