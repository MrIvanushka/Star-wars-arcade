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
    public:
        AI(GameObject* obj) : Component(obj) {}

        void update(float deltaTime) override {
            _stateMachine->update(deltaTime);
        }

};

class JediAI : public AI
{
    private:

    public:
        JediAI(GameObject* obj) : AI(obj) {}

        void setupStateMachine(NavMeshAgent* character, std::vector<glm::vec3> wanderingTargets, Observable** attackScript, Collider* visionCollider);
};
/*
class SquadLeader : public AI
{
    private:

    public:
        SquadLeader(GameObject* obj) : AI(obj) {}

        void setupStateMachine(NavMeshAgent* character, GameObject* target);
};

class SquadMember : public AI
{
    private:
        GameObject* sq_leader;

    public:
        SquadMember(GameObject* obj, GameObject* leader) : AI(obj), sq_leader(leader) {}

        void setupStateMachine(NavMeshAgent* character, GameObject* target);
};*/

#endif // CHARACTERSTATEMACHINES_H
