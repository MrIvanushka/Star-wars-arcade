#ifndef CHARACTERSTATES_H
#define CHARACTERSTATES_H

#include <vector>
#include <random>

#include "../Utilities/StateMachine.h"
#include "../Utilities/Observer.h"
#include "../Physics/Collider.h"
#include "../Physics/octree.h"
#include "../BaseComponents/Animator.h"
#include "../GameComponents/IDamageable.h"
#include "../GameComponents/FractionMember.h"
#include "../GameComponents/Blaster.h"
#include "NavMeshAgent.hpp"

#include <iostream>

static constexpr const int seed = 42;
static constexpr const float MAX_POSITION = 1000, MIN_POSITION = 0; // Field size


class ITargetHunter {
public:
    virtual void setTarget(GameObject* target) = 0;
};

class TargetHuntState : public State, public ITargetHunter{
public:
    TargetHuntState() = default;
    
    TargetHuntState(std::vector<Transition*> transitions) : State(transitions) {}

};

class TargetHuntTransition : public Transition, public ITargetHunter{
public:
    TargetHuntTransition(State* nextState) : Transition(nextState) {}
};

class SeeTargetTransition : public Transition
{
private:
    Collider* _visionCollider;
    Fraction _selfFraction;
    std::vector<ITargetHunter*> _hunters;
public:
    SeeTargetTransition(State* nextState, Collider* visionCollider, Fraction selfFraction, std::vector<ITargetHunter*> hunters) : 
        Transition(nextState), _visionCollider(visionCollider), _selfFraction(selfFraction), _hunters(hunters)
    {}

    bool needTransit() override {
        auto touchedColliders = _visionCollider->getTouchedColliders();

        for(auto collider : touchedColliders)
        {
            FractionMember* fractionMember = collider->getComponent<FractionMember>();

            if(fractionMember != nullptr && collider->isEnabled()){
                if(fractionMember->getFraction() != _selfFraction){
                    for(int i = 0; i < _hunters.size(); i++)
                        _hunters[i]->setTarget(collider->getGameObject());
                    std::cout << "See target" << std::endl;
                    return true;
                }
            }
        }

        return false;
    }   
};

class LowDistanceTransition : public Transition
{
private:
    NavMeshAgent* m_character;
    float _requiredDistance;
public:
    LowDistanceTransition(State* nextState, NavMeshAgent* character, float requiredDistance) : 
        Transition(nextState), m_character(character), _requiredDistance(requiredDistance)
    {}

    bool needTransit() override {
        return (m_character->getDistance() < _requiredDistance || !m_character->hasPath());
    }   
};

class KillTransition : public TargetHuntTransition
{
    private:
        IDamageable* m_target;

    public:
        KillTransition(State* nextState) :
            TargetHuntTransition(nextState) {}

        void setTarget(GameObject* newTarget) { m_target = newTarget->getComponent<IDamageable>(); }

        bool needTransit() override {
            if(!m_target->isAlive())
                std::cout << "killed" << std::endl;

            return !m_target->isAlive();
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

class FollowState : public TargetHuntState
{
    private:
        NavMeshAgent* m_character;
        OrientedPoint* m_target;

    public:
        FollowState(NavMeshAgent* character, const std::vector<Transition*>& transitions) :
            TargetHuntState(transitions), m_character(character) {}

        void setTarget(GameObject* newTarget) override { m_target = newTarget; }
        
        void update(float deltaTime) override {
                m_character->setDestination(m_target->getPosition());
        }
};

class ShootingState : public TargetHuntState
{
    private:
        Blaster* _blaster;
        OrientedPoint* m_target;
    public:
        ShootingState(Blaster* blaster, const std::vector<Transition*>& transitions) :
            TargetHuntState(transitions), _blaster(blaster) {}

        void setTarget(GameObject* newTarget) override { m_target = newTarget; }
        
        void start(){
            _blaster->shoot(m_target->getPosition());
        }
};

class RaycastTransition : public TargetHuntTransition {
    private:
        Octree::node* _collisionProcessor;
        OrientedPoint* m_character;
        OrientedPoint* m_target;

    public:
        RaycastTransition(State* nextState, OrientedPoint* character, Octree::node* collisionProcessor) :
            TargetHuntTransition(nextState), _collisionProcessor(collisionProcessor), m_character(character) {}

        void setTarget(GameObject* newTarget) { m_target = newTarget; }

        bool needTransit() override {
            auto delta = glm::normalize(m_target->getPosition() - m_character->getPosition());

            Ray ray(m_character->getPosition(), delta);
            float dist = 1000;

            auto foundRegion = _collisionProcessor->checkCollisionsRay(ray, dist);

            if(foundRegion != nullptr)
            {
                std::cout << "something found " << foundRegion->center.x << " " << foundRegion->center.y << " " << foundRegion->center.z << " dist " << dist << std::endl;
                std::cout << "self pos " << m_character->getPosition().x << " " << m_character->getPosition().y << " " << m_character->getPosition().z << std::endl;
                std::cout << "target pos " << m_target->getPosition().x << " " << m_target->getPosition().y << " " << m_target->getPosition().z << std::endl;

                if(foundRegion->collider->getComponent<IDamageable>())
                    return true;
            }

            return false;
        }
};

#endif // CHARACTERSTATES_H
