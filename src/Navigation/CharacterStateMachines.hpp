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

        void setupStateMachine(NavMeshAgent* character, OrientedPoint* target, Character* target_character);
};

class SquadLeader : public AI
{
    private:

    public:
        SquadLeader(GameObject* obj/*, Vision* vision*/) : AI(obj/*, vision*/) {}

        void setupStateMachine(NavMeshAgent* character, OrientedPoint* target, Character* target_character/*, Weapon* weapon*/);
};

class SquadMember : public AI
{
    private:
        Character* sq_leader;

    public:
        SquadMember(GameObject* obj, Character* leader/*, Vision* vision*/) : AI(obj/*, vision*/), sq_leader(leader) {}

        void setupStateMachine(NavMeshAgent* character, OrientedPoint* target, OrientedPoint* leader, Character* target_character/*, Weapon* weapon*/);
};

#endif // CHARACTERSTATEMACHINES_H

//Character -> Idamagable
//NavMeshAgent remove slowdown
//y ==> z
