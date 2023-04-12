#include "NavMeshAgent.hpp"

NavMeshAgent::NavMeshAgent(GameObject* object, NavMeshSurface* surface, CharacterController* ctrl) : 
    Component(object), m_surface(surface), m_cctrl(ctrl)
{
    m_path = NavMeshPath();
    current_destination = gameObject->getPosition();
    stop_distance = 0.f;
}

void NavMeshAgent::setDestination(glm::vec3 point) {
    current_destination = point;
    m_path = m_surface->get_path({gameObject->getPosition().x, gameObject->getPosition().y}, {current_destination.x, current_destination.y});
}

void NavMeshAgent::removeDestination() {
    m_path = NavMeshPath();
    current_destination = gameObject->getPosition();
}

void NavMeshAgent::setStopDistance(double new_stop_distance) {
    stop_distance = new_stop_distance;
}

void NavMeshAgent::update(float deltaTime) {
    m_cctrl->update(deltaTime);
}

bool NavMeshAgent::hasPath() {
    if (gameObject->getPosition() == current_destination) { m_path = NavMeshPath(); }
    return (m_path.get_points().size() != 0);
}
