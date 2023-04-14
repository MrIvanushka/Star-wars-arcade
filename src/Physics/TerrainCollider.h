#ifndef SWTOR_TERRAINCOLLIDER_H
#define SWTOR_TERRAINCOLLIDER_H

#include"meshcollider.h"


class TerrainCollider : public Collider {
private:
    std::vector<std::vector<glm::vec3>> heights;

public:
    TerrainCollider(GameObject* obj) : Collider(obj)

    ~TerrainCollider() = default;

    std::vector<Face> getFacesInRegion(BoundingRegion br);

    std::vector<Face> getFacesInRegion(glm::vec3 center, float radius);
};

#endif