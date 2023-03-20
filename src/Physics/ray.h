#ifndef RAY_H
#define RAY_H

#include <../../glm/glm/glm.hpp>

#include "bounds.h"
#include "meshcollider.h"
#include "../Engine/GameObject.h"

class Ray {
public:
	glm::vec3 origin;
	glm::vec3 dir;
	glm::vec3 invdir;

	Ray(glm::vec3 origin, glm::vec3 dir);

	bool intersectsBoundingRegion(BoundingRegion br, float &tmin, float &tmax);
	bool intersectsMesh(MeshCollider* mesh, OrientedPoint* instance, float &t);
};

#endif