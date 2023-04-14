#ifndef CHARACTERSTATES_H
#define CHARACTERSTATES_H

#include <vector>
#include <random>

#include "../Utilities/StateMachine.h"
#include "../Physics/Collider.h"
#include "../BaseComponents/Animator.h"
#include "../GameComponents/Character.h"

#include "NavMeshAgent.hpp"

constexpr const int seed = 42;
constexpr const float MAX_POSITION = 1000, MIN_POSITION = 0; // Field size
constexpr const float ATTACK_DISTANCE = 10;
constexpr const float SEE_DISTANCE = 100;

enum Fraction {
    Jedi,
    Sith,
};

class Vision;

class SeeTargetTransition : public Transition
{
    private:
        Vision* m_vision;
        Fraction fraction_member;
    
    public:
        SeeTargetTransition(State* nextState, Vision* m_vision, Fraction fraction);

        void setVision(Vision* new_vision) { m_vision = new_vision; }

        void setFraction(Fraction fraction) { m_fraction = fraction; }
        
        void onEnable() override;

        void update(float deltaTime) override;

        bool needTransit() override;
};

class KillTransition : public Transition
{
    private:
        Character* m_character;

    public:
        KillTransition(State* nextState, Character* character);

        void setCharacter(Character* new_character) { m_character = new_character; }

        void onEnable() override;

        void update(float deltaTime) override;

        bool needTransit() override;
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

class AttackState : public State
{
    private:
        Animator* m_animator;

    public:
        AttackState(Animator* animator, const std::vector<Transition*>& transitions);

        void setAnimator(Animator* new_animator) { m_animator = new_animator; }
        
        void start() override;
        
        void update(float deltaTime) override;
};

class FollowState : public State
{
    private:
        NavMeshAgent* m_character;
        NavMeshAgent* m_target;

    
    public:
        FollowState(NavMeshAgent* character, NavMeshAgent* target, const std::vector<Transition*>& transitions);

        void setTarget(NavMeshAgent* new_target) { m_target = new_target; }
        
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

#endif // CHARACTERSTATES_H
