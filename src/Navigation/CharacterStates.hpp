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

class DistanceTransition : public Transition
{
private:
    NavMeshAgent* m_character;
    float _requiredDistance;
public:
    DistanceTransition(State* nextState, NavMeshAgent* character, float requiredDistance) : 
        Transition(nextState), m_character(character), _requiredDistance(requiredDistance)
    {}

    bool needTransit() override {
        return m_character->getDistance() < _requiredDistance;
    }   
};

class KillTransition : public Transition
{
    private:
        IDamageable* m_character;

    public:
        KillTransition(State* nextState, IDamageable* character = nullptr) :
            Transition(nextState), m_character(character) {}

        void setCharacter(IDamageable* new_character) { m_character = new_character; }

        bool needTransit() override {
            return !m_character->isAlive();
        }
};

class WanderingState : public State 
{
    private:
        NavMeshAgent* m_navMeshAgent;
        std::vector<glm::vec3> _wanderingPoints;

        std::mt19937_64 eng;
        std::uniform_int_distribution<std::mt19937::result_type> dist6;
    public:
        WanderingState(NavMeshAgent* navMeshAgent, std::vector<glm::vec3> wanderingPoints, const std::vector<Transition*>& transitions) :
            State(transitions), m_navMeshAgent(navMeshAgent), _wanderingPoints(wanderingPoints), eng(seed) {}

        void start() override {
                m_navMeshAgent->setDestination(_wanderingPoints[dist6(eng) % _wanderingPoints.size()]);
        }

        void update(float deltaTime) override {
            while (!m_navMeshAgent->hasPath()) {
                m_navMeshAgent->setDestination(_wanderingPoints[dist6(eng) % _wanderingPoints.size()]);
            }
        }
};

class PatrollingState : public State 
{
    private:
        NavMeshAgent* m_navMeshAgent;
        std::vector<glm::vec3> _wanderingPoints;

        std::mt19937_64 eng;
        std::uniform_int_distribution<std::mt19937::result_type> dist6;
    
    public:
        PatrollingState(NavMeshAgent* navMeshAgent, std::vector<glm::vec3> wanderingPoints, const std::vector<Transition*>& transitions) :
            State(transitions),  m_navMeshAgent(navMeshAgent), _wanderingPoints(wanderingPoints), eng(seed), dist6(MIN_POSITION, MAX_POSITION) {}

        void start() override {
            m_navMeshAgent->setDestination(_wanderingPoints[dist6(eng) % _wanderingPoints.size()]);
        }
};

class AttackState : public State, public Observable
{
    public:
        AttackState() {}

        AttackState(const std::vector<Transition*>& transitions) :
            State(transitions) {}
        
        void start() override {
            invoke();
        }
};

class FollowState : public State
{
    private:
        NavMeshAgent* m_character;
        OrientedPoint* m_target;

    public:
        FollowState(NavMeshAgent* character, const std::vector<Transition*>& transitions) :
            State(transitions), m_character(character) {}

        void setTarget(OrientedPoint* new_target) { m_target = new_target; }
        
        void update(float deltaTime) override {
            m_character->setDestination(m_target->getPosition());
        }
};


#endif // CHARACTERSTATES_H
