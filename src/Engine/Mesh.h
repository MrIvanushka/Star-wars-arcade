//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_MESH_H
#define SWTOR_MESH_H

#include<iostream>
#include<vector>

#include"Vertex.h"
#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"GameObject.h"
#include"../Physics/bounds.h"

class Mesh
{
private:
    Vertex * vertexArray;
    unsigned nrOfVertices;
    GLuint* indexArray;
    unsigned nrOfIndices;

    OrientedPoint* centerPoint;
    glm::vec3 scale;
    glm::mat4 ModelMatrix;

    void updateModelMatrix();

public:
    friend class MeshRenderer;
    friend class SkinnedMeshRenderer;

    Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, OrientedPoint* orientedPoint);

    Mesh(const Mesh& obj);

    ~Mesh();
};

#endif //SWTOR_MESH_H
