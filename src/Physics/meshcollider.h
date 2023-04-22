#ifndef COLLISIONMESH_H
#define COLLISIONMESH_H

#include <vector>

#include "Collider.h"
#include "../Engine/Vertex.h"

// forward declarations
class MeshCollider;

typedef struct Face {
	MeshCollider* mesh;
	unsigned int i1, i2, i3;

	glm::vec3 baseNormal;
	glm::vec3 norm;

	bool collidesWithFace(OrientedPoint* thisRB, struct Face& face, OrientedPoint* faceRB, glm::vec3& retNorm, std::vector<glm::vec3>& touchFace);
	bool collidesWithSphere(OrientedPoint* thisRB, BoundingRegion& br, glm::vec3& retNorm);
} Face;

class MeshCollider : public Collider {
	BoundingRegion br;
	std::vector<glm::vec3> points;
	std::vector<Face> faces;
public:
	friend class Octree::node;
	friend class Face;
	friend class Ray;

	MeshCollider(GameObject* object) : Collider(object) {}
	~MeshCollider() = default;
	void initialize(unsigned int noPoints, float* coordinates, unsigned int noFaces, unsigned int* indices);
	void initialize(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	std::vector<Face> getFaces(){
		return faces;
	}

	BoundingRegion* getRegion(){
		return &br;
	}
};

#endif