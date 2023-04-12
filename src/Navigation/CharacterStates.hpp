#ifndef CHARACTERSTATES_H
#define CHARACTERSTATES_H

#include <vector>
#include <random>

#include "../Utilities/StateMachine.h"
#include "../Physics/Collider.h"
#include "../BaseComponents/Animator.h"
#include "../GameComponents/Character.h"

#include "NavMeshAgent.hpp"

static constexpr auto seed = 42;
static constexpr auto MAX_POSITION = 1000, MIN_POSITION = 0; // Field size

enum Fraction {
    Jedi,
    Sith,
};

class WanderingState : public State 
{
    private:
        NavMeshAgent* m_navMeshAgent;

        std::mt19937_64 eng;
        std::uniform_int_distribution<std::mt19937::result_type> dist6;
    
    public:
        WanderingState(NavMeshAgent* navMeshAgent, const std::vector<Transition*>& transitions);

        void start() override;

        void update(float deltaTime) override;
};

class PatrollingState : public State 
{
    private:
        NavMeshAgent* m_navMeshAgent;

        std::mt19937_64 eng;
        std::uniform_int_distribution<std::mt19937::result_type> dist6;
    
    public:
        PatrollingState(NavMeshAgent* navMeshAgent, const std::vector<Transition*>& transitions);

        void start() override;

        void update(float deltaTime) override;
};

class SeeTargetTransition : public Transition
{
    private:
        Collider* m_collider;
        Fraction fraction_member;
    
    public:
        SeeTargetTransition(State* nextState, Collider* collider, Fraction fraction);

        void onEnable() override;

        void update(float deltaTime) override;

        bool needTransit() override;
};

class AttackState : public State
{
    private:
        Animator* m_animator;

    public:
        AttackState(Animator* animator, const std::vector<Transition*>& transitions);

        void start() override;
        
        void update(float deltaTime) override;
};

class FollowState : public State
{
    private:
        NavMeshAgent* m_target;
    
    public:
        FollowState(NavMeshAgent* target, const std::vector<Transition*>& transitions);

        void start() override;
        
        void update(float deltaTime) override;
};

class ShootingState : public State
{
    private:
        /*Weapon* m_weapon;*/

    public:
        ShootingState(/*Weapon* weapon, */const std::vector<Transition*>& transitions);

        void start() override;
        
        void update(float deltaTime) override;
};

class KillTransition : public Transition
{
    private:
        Character* m_character;

    public:
        KillTransition(State* nextState, Character* character);

        void onEnable() override;

        void update(float deltaTime) override;

        bool needTransit() override;
};

#endif // CHARACTERSTATES_H
