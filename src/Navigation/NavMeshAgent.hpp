#ifndef NAVMESHAGENT_H
#define NAVMESHAGENT_H

#include "../Engine/GameObject.h"
#include "../GameComponents/CharacterController.h"

#include "NavMesh.hpp"

class NavMeshAgent : public Component
{
    private:
        NavMeshSurface* m_surface;
        NavMeshPath m_path;
        glm::vec3 current_destination;
        double stop_distance;
        CharacterController* m_cctrl;

    public:
        NavMeshAgent(GameObject* object, NavMeshSurface* surface, CharacterController* ctrl);

        void setDestination(glm::vec3 point);

        void removeDestination();

        void setStopDistance(double new_stop_distance);

        void update(float deltaTime);

        bool hasPath();
};

#endif // NAVMESHAGENT_H