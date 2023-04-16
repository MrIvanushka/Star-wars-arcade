#include "NavMeshAgent.hpp"

NavMeshAgent::NavMeshAgent(GameObject* object) : 
    Component(object)
{
    m_path = NavMeshPath();
    current_destination = gameObject->getPosition();
    passed_distance = 0.f;
    stop_distance = 0.f;
    current_index = 0;
}

void NavMeshAgent::setDestination(glm::vec3 point) {
    current_destination = point;
    m_path = m_surface->get_path({gameObject->getPosition().x, gameObject->getPosition().y}, {current_destination.x, current_destination.y});
    m_cctrl->slowDown(ACCELERATION_TIME);

    current_index = 1; // [0] point is current position
    if (hasPath()) { m_cctrl->accelerate(glm::vec3({m_path[current_index].x, m_path[current_index].y, 0.0f}) - gameObject->getPosition(), ACCELERATION_TIME); }
}

void NavMeshAgent::removeDestination() {
    m_path = NavMeshPath();
    current_destination = gameObject->getPosition();
    m_cctrl->slowDown(ACCELERATION_TIME);
}

void NavMeshAgent::setStopDistance(double new_stop_distance) {
    stop_distance = new_stop_distance;
}

glm::vec3 NavMeshAgent::getPosition() {
    return gameObject->getPosition();
}

void NavMeshAgent::start() {
    m_cctrl = gameObject->getComponent<CharacterController>();
}

void NavMeshAgent::update(float deltaTime) {
    if (hasPath())
    {
        if (glm::distance(glm::vec3({m_path[current_index].x, m_path[current_index].y, 0.0f}), glm::vec3({m_path[current_index - 1].x, m_path[current_index - 1].y, 0.0f})) < 
            glm::distance(glm::vec3({m_path[current_index - 1].x, m_path[current_index - 1].y, 0.0f}), gameObject->getPosition())) 
        {
            m_cctrl->slowDown(ACCELERATION_TIME);
            if (current_index < m_path.get_points().size() - 1)
            {
                ++current_index;
                m_cctrl->accelerate(glm::vec3({m_path[current_index].x, m_path[current_index].y, 0.0f}) - gameObject->getPosition(), ACCELERATION_TIME);
            }
            else
            {
                m_path = NavMeshPath();
            }
        }
    }
}

bool NavMeshAgent::hasPath() {
    if (gameObject->getPosition() == current_destination) { m_path = NavMeshPath(); } //???
    return (m_path.get_points().size() != 0);
}
