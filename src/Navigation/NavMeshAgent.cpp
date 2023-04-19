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

void NavMeshAgent::addSurface(NavMeshSurface* surface)
{
    m_surface = surface;
}

void NavMeshAgent::setDestination(glm::vec3 point) {
    current_destination = point;
    m_path = m_surface->get_path({gameObject->getPosition().x, gameObject->getPosition().z}, {current_destination.x, current_destination.z});

    current_index = 1; // [0] point is current position
}

void NavMeshAgent::removeDestination() {
    m_path = NavMeshPath();
    current_destination = gameObject->getPosition();
}

void NavMeshAgent::setStopDistance(double new_stop_distance) {
    stop_distance = new_stop_distance;
}

float NavMeshAgent::getDistance() {
    if(!hasPath())
        return 0;
    else {
        float dist = glm::distance(glm::vec3({m_path[current_index].x, 0.0f, m_path[current_index].y}), gameObject->getPosition());

        for(int i = current_index + 1; i < m_path.get_points().size(); i++)
            dist += glm::distance(glm::vec3({m_path[current_index].x, 0.0f, m_path[current_index].y}), glm::vec3({m_path[current_index - 1].x, 0.0f, m_path[current_index - 1].y}));

        return dist;
    }
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
        m_cctrl->accelerate(glm::normalize(glm::vec3(m_path[current_index].x - gameObject->getPosition().x, 0.0f, m_path[current_index].y - gameObject->getPosition().z)), deltaTime);

        if (glm::distance(glm::vec3({m_path[current_index].x, 0.0f, m_path[current_index].y}), glm::vec3({m_path[current_index - 1].x, 0.0f, m_path[current_index - 1].y})) < 
            glm::distance(glm::vec3({m_path[current_index - 1].x, 0.0f, m_path[current_index - 1].y}), gameObject->getPosition())) 
        {
            if (current_index < m_path.get_points().size() - 1)
            {
                ++current_index;
            }
            else
            {
                m_path = NavMeshPath();
            }
        }
    }
    else {
        m_cctrl->slowDown(deltaTime);
    }
}

bool NavMeshAgent::hasPath() {
    if (glm::distance(gameObject->getPosition(), current_destination) < stop_distance) { m_path = NavMeshPath(); } //???
    return (m_path.get_points().size() != 0);
}
