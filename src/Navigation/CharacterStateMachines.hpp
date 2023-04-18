#ifndef CHARACTERSTATEMACHINES_H
#define CHARACTERSTATEMACHINES_H

#include "../Engine/GameObject.h"
#include "../Physics/Collider.h"
#include "../GameComponents/IDamageable.h"

#include "CharacterStates.hpp"

class AI : public Component 
{
    protected:
        StateMachine* _stateMachine;
    
    private:
        //Vision* m_vision;

    public:
        AI(GameObject* obj/*, Vision* vision*/) : Component(obj)/*, m_vision(vision)*/ {}

        void update(float deltaTime) override {
            _stateMachine->update(deltaTime);
        }

};

class Jedi : public AI
{
    private:

    public:
        Jedi(GameObject* obj/*, Vision* vision*/) : AI(obj/*, vision*/) {}

        void setupStateMachine(NavMeshAgent* character, GameObject* target);
};

class SquadLeader : public AI
{
    private:

    public:
        SquadLeader(GameObject* obj/*, Vision* vision*/) : AI(obj/*, vision*/) {}

        void setupStateMachine(NavMeshAgent* character, GameObject* target/*, Weapon* weapon*/);
};

class SquadMember : public AI
{
    private:
        GameObject* sq_leader;

    public:
        SquadMember(GameObject* obj, GameObject* leader/*, Vision* vision*/) : AI(obj/*, vision*/), sq_leader(leader) {}

        void setupStateMachine(NavMeshAgent* character, GameObject* target/*, Weapon* weapon*/);
};

#endif // CHARACTERSTATEMACHINES_H

//Character -> Idamagable
//NavMeshAgent remove slowdown
//y ==> z
