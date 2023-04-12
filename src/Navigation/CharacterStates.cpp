#include "CharacterStates.hpp"

/* WanderingState */

WanderingState::WanderingState(NavMeshAgent* navMeshAgent, const std::vector<Transition*>& transitions) :
    m_navMeshAgent(navMeshAgent), State(transitions), eng(seed), dist6(MIN_POSITION, MAX_POSITION) {}

void WanderingState::start() {
    while (!m_navMeshAgent->hasPath()) {
        m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
    }
}

void WanderingState::update(float deltaTime) {
    while (!m_navMeshAgent->hasPath()) {
        m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
    }
}

/* PatrollingState */

PatrollingState::PatrollingState(NavMeshAgent* navMeshAgent, const std::vector<Transition*>& transitions) :
    m_navMeshAgent(navMeshAgent), State(transitions), eng(seed), dist6(MIN_POSITION, MAX_POSITION) {}

void PatrollingState::start() {
    while (!m_navMeshAgent->hasPath()) {
        m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
    }
}

void PatrollingState::update(float deltaTime) {
    while (!m_navMeshAgent->hasPath()) {
        m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
    }
}

/* SeeTargetTransition */

SeeTargetTransition::SeeTargetTransition(State* nextState, Collider* collider, Fraction fraction) :
    Transition(nextState), m_collider(collider), fraction_member(fraction)
{
    // TODO
}

void SeeTargetTransition::onEnable() {
    // TODO
}

void SeeTargetTransition::update(float deltaTime) {
    // TODO
}

bool SeeTargetTransition::needTransit() {
    // TODO
    return false;
}

/* AttackState */

AttackState::AttackState(Animator* animator, const std::vector<Transition*>& transitions) :
    m_animator(animator), State(transitions) {}

void AttackState::start() {}

void AttackState::update(float deltaTime) {
    m_animator->update(deltaTime);
}

/* FollowState */

FollowState::FollowState(NavMeshAgent* target, const std::vector<Transition*>& transitions) :
    m_target(target), State(transitions) {}

void FollowState::start() {}

void FollowState::update(float deltaTime) {}

/* ShootingState */

//TODO: Implementation

/* KillTransition */

KillTransition::KillTransition(State* nextState, Character* character) :
    Transition(nextState), m_character(character) {}

void KillTransition::onEnable() {}

void KillTransition::update(float deltaTime) {}

bool KillTransition::needTransit() {
    return m_character->IsDead();
}

