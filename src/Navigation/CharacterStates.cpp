#include "CharacterStates.hpp"

/* SeeTargetTransition */

SeeTargetTransition::SeeTargetTransition(State* nextState, NavMeshAgent* character, OrientedPoint* target, /*Vision* vision,*/ Fraction fraction) :
    Transition(nextState), m_character(character), m_target(target), /*m_vision(vision),*/ fraction_member(fraction)
{}

void SeeTargetTransition::onEnable() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void SeeTargetTransition::update(float deltaTime) {

}

bool SeeTargetTransition::needTransit() {
    return (glm::distance(m_target->getPosition(), m_character->getPosition()) < SEE_DISTANCE);
}

/* LeaderDiedTransition */

LeaderDiedTransition::LeaderDiedTransition(State* nextState, IDamageable* character) :
    Transition(nextState), m_character(character) {}

void LeaderDiedTransition::onEnable() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void LeaderDiedTransition::update(float deltaTime) {}

bool LeaderDiedTransition::needTransit() {
    return !m_character->isAlive();
}

/* KillTransition */

KillTransition::KillTransition(State* nextState, IDamageable* character) :
    Transition(nextState), m_character(character) {}

void KillTransition::onEnable() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void KillTransition::update(float deltaTime) {}

bool KillTransition::needTransit() {
    return !m_character->isAlive();
}

/* WanderingState */

WanderingState::WanderingState(NavMeshAgent* navMeshAgent, const std::vector<Transition*>& transitions) :
    m_navMeshAgent(navMeshAgent), State(transitions), eng(seed), dist6(MIN_POSITION, MAX_POSITION) {}

void WanderingState::start() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

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
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    while (!m_navMeshAgent->hasPath()) {
        m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
    }
}

void PatrollingState::update(float deltaTime) {
    while (!m_navMeshAgent->hasPath()) {
        m_navMeshAgent->setDestination(glm::vec3(dist6(eng), dist6(eng), dist6(eng)));
    }
}

/* AttackState */

AttackState::AttackState(const std::vector<Transition*>& transitions) :
    State(transitions) {}

void AttackState::start() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    invoke();
}

/* FollowState */

FollowState::FollowState(NavMeshAgent* character, OrientedPoint* target, const std::vector<Transition*>& transitions) :
    m_target(target), m_character(character), State(transitions) {}

void FollowState::start() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void FollowState::update(float deltaTime) {
    m_character->setDestination(m_target->getPosition());
}

/* ShootingState */

ShootingState::ShootingState(/*Weapon* weapon, */const std::vector<Transition*>& transitions) :
    State(transitions)
{}

void ShootingState::start() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    //m_weapon->start();
}

void ShootingState::update(float deltaTime) {
    //m_weapon->update(deltaTime);
}

