#ifndef CHARACTERSTATES_H
#define CHARACTERSTATES_H

#include <vector>
#include <random>

#include "../Utilities/StateMachine.h"
#include "../Utilities/Observer.h"
#include "../Physics/Collider.h"
#include "../BaseComponents/Animator.h"
#include "../GameComponents/IDamageable.h"
#include "NavMeshAgent.hpp"

#include <iostream>

static constexpr const int seed = 42;
static constexpr const float MAX_POSITION = 1000, MIN_POSITION = 0; // Field size

static constexpr const float ATTACK_DISTANCE = 10;
static constexpr const float SEE_DISTANCE = 100;
static constexpr const float ATTACK_DURATION = 1;

static constexpr const float SHOOTING_DISTANCE = 100;
static constexpr const float SHOOTING_DURATION = 1;

enum Fraction {
    None,
    Jedi,
    Sith,
};

class Vision;

class Weapon;

class SeeTargetTransition : public Transition
{
    private:
        //Vision* m_vision;
        NavMeshAgent* m_character;
        OrientedPoint* m_target;
        Fraction fraction_member;
    
    public:
        SeeTargetTransition(State* nextState, NavMeshAgent* character, OrientedPoint* target, /*Vision* vision,*/ Fraction fraction) :
            Transition(nextState), m_character(character), m_target(target), /*m_vision(vision),*/ fraction_member(fraction) {}

        //void setVision(Vision* new_vision) { m_vision = new_vision; }

        void setFraction(Fraction fraction) { fraction_member = fraction; }
        
        void onEnable() override {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        }

        void update(float deltaTime) override {}

        bool needTransit() override{
            return (glm::distance(m_target->getPosition(), m_character->getPosition()) < SEE_DISTANCE);
        }
};

class AttackDurationTransition : public Transition
{
private:
    float _duration;
    float _currentTime;
public:
    AttackDurationTransition(State* nextState) : Transition(nextState), _duration(ATTACK_DURATION)
    {}

    void onEnable() override
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;

        _currentTime = 0;
    }

    void update(float deltaTime) override
    {
        _currentTime += deltaTime;
    }

    bool needTransit() override
    {
        return _currentTime > _duration;
    }  
};


class AttackDistanceTransition : public Transition
{
private:
    NavMeshAgent* m_character;
    OrientedPoint* m_target;

public:
    AttackDistanceTransition(State* nextState, NavMeshAgent* character, OrientedPoint* target) : 
        Transition(nextState), m_character(character), m_target(target)
    {}

    void onEnable() override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }


    bool needTransit() override {
        return (glm::distance(m_target->getPosition(), m_character->getPosition()) < ATTACK_DISTANCE);
    }   
};

class ShootingDurationTransition : public Transition
{
private:
    float _duration;
    float _currentTime;
public:
    ShootingDurationTransition(State* nextState) : Transition(nextState), _duration(SHOOTING_DURATION)
    {}

    void onEnable() override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;

        _currentTime = 0;
    }

    void update(float deltaTime) override {
        _currentTime += deltaTime;
    }

    bool needTransit() override
    {
        return _currentTime > _duration;
    }  
};

class ShootingDistanceTransition : public Transition
{
private:
    NavMeshAgent* m_character;
    OrientedPoint* m_target;

public:
    ShootingDistanceTransition(State* nextState, NavMeshAgent* character, OrientedPoint* target) : 
        Transition(nextState), m_character(character), m_target(target)
    {}

    void onEnable() override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }


    bool needTransit() override {
        return (glm::distance(m_target->getPosition(), m_character->getPosition()) < SHOOTING_DISTANCE);
    }   
};

class LeaderDiedTransition : public Transition
{
    private:
        IDamageable* m_character;

    public:
        LeaderDiedTransition(State* nextState, IDamageable* character) :
            Transition(nextState), m_character(character) {}

        void setCharacter(IDamageable* new_character) { m_character = new_character; }

        void onEnable() override {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        }

        void update(float deltaTime) override {}

        bool needTransit() override {
            return !m_character->isAlive();
        }
};

class KillTransition : public Transition
{
    private:
        IDamageable* m_character;

    public:
        KillTransition(State* nextState, IDamageable* character) :
            Transition(nextState), m_character(character) {}

        void setCharacter(IDamageable* new_character) { m_character = new_character; }

        void onEnable() override {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        }

        void update(float deltaTime) override {}

        bool needTransit() override {
            return !m_character->isAlive();
        }
};

class WanderingState : public State 
{
    private:
        NavMeshAgent* m_navMeshAgent;

        std::mt19937_64 eng;
        std::uniform_int_distribution<std::mt19937::result_type> dist6;
    
    public:
        WanderingState(NavMeshAgent* navMeshAgent, const std::vector<Transition*>& transitions) :
            m_navMeshAgent(navMeshAgent), State(transitions), eng(seed), dist6(MIN_POSITION, MAX_POSITION) {}

        void start() override {
            std::cout << __PRETTY_FUNCTION__ << std::endl;

            while (!m_navMeshAgent->hasPath()) {
                m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
            }
        }

        void update(float deltaTime) override {
            while (!m_navMeshAgent->hasPath()) {
                m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
            }
        }
};

class PatrollingState : public State 
{
    private:
        NavMeshAgent* m_navMeshAgent;

        std::mt19937_64 eng;
        std::uniform_int_distribution<std::mt19937::result_type> dist6;
    
    public:
        PatrollingState(NavMeshAgent* navMeshAgent, const std::vector<Transition*>& transitions) :
            m_navMeshAgent(navMeshAgent), State(transitions), eng(seed), dist6(MIN_POSITION, MAX_POSITION) {}

        void start() override {
            std::cout << __PRETTY_FUNCTION__ << std::endl;

            while (!m_navMeshAgent->hasPath()) {
                m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
            }
        }

        void update(float deltaTime) override {
            while (!m_navMeshAgent->hasPath()) {
                m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
            }
        }
};

class AttackState : public State, public Observable
{
    public:
        AttackState() {}

        AttackState(const std::vector<Transition*>& transitions) :
            State(transitions) {}
        
        void start() override {
            std::cout << __PRETTY_FUNCTION__ << std::endl;

            invoke();
        }
};

class FollowState : public State
{
    private:
        NavMeshAgent* m_character;
        OrientedPoint* m_target;

    public:
        FollowState(NavMeshAgent* character, OrientedPoint* target, const std::vector<Transition*>& transitions) :
            m_target(target), m_character(character), State(transitions) {}

        void setTarget(OrientedPoint* new_target) { m_target = new_target; }
        
        void start() override {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        }
        
        void update(float deltaTime) override {
            m_character->setDestination(m_target->getPosition());
        }
};

class ShootingState : public State
{
    private:
        /*Weapon* m_weapon;*/

    public:
        ShootingState(/*Weapon* weapon, */const std::vector<Transition*>& transitions) :
            State(transitions) {}
        
        //void setWeapon(Weapon* weapon);
        
        void start() override {
            std::cout << __PRETTY_FUNCTION__ << std::endl;

            //m_weapon->shoot();
        }
        
        void update(float deltaTime) override  {
            //m_weapon->update(deltaTime);
        }
};

#endif // CHARACTERSTATES_H
