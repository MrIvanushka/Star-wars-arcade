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
        size_t current_index;
        glm::vec3 current_destination;
        double passed_distance;
        double stop_distance = 0.5;

        CharacterController* m_cctrl;

    public:
        NavMeshAgent(GameObject* object);

        void addSurface(NavMeshSurface* surface);

        void setDestination(glm::vec3 point);

        void removeDestination();

        void setStopDistance(double new_stop_distance);

        float getDistance();

        glm::vec3 getPosition();

        void start() override;

        void update(float deltaTime) override;

        bool hasPath();
};

#endif // NAVMESHAGENT_H