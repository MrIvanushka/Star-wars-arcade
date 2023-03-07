//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_MESH_H
#define SWTOR_MESH_H

#include<iostream>
#include<vector>

#include"Vertex.h"
#include"Primitives.h"
#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"GameObject.h"


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

    friend class MeshRenderer;

    void updateModelMatrix()
    {
        this->ModelMatrix = glm::mat4(1.f);
        this->ModelMatrix = glm::translate(this->ModelMatrix, this->centerPoint->getPosition());
        glm::quat rotation = this->centerPoint->getRotation();
        rotation.x = -rotation.x;
        rotation.y = -rotation.y;
        rotation.z = -rotation.z;
        this->ModelMatrix *= glm::mat4_cast(rotation);
        this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
    }

public:
    Mesh(
            Vertex* vertexArray,
            const unsigned& nrOfVertices,
            GLuint* indexArray,
            const unsigned& nrOfIndices,
            OrientedPoint* orientedPoint,
            glm::vec3 scale = glm::vec3(1.f))
    {
        this->centerPoint = orientedPoint;
        this->scale = scale;

        this->nrOfVertices = nrOfVertices;
        this->nrOfIndices = nrOfIndices;

        this->vertexArray = new Vertex[this->nrOfVertices];
        for (size_t i = 0; i < nrOfVertices; i++)
        {
            this->vertexArray[i] = vertexArray[i];
        }

        this->indexArray = new GLuint[this->nrOfIndices];
        for (size_t i = 0; i < nrOfIndices; i++)
        {
            this->indexArray[i] = indexArray[i];
        }

        this->updateModelMatrix();
    }

    Mesh(
            Primitive* primitive,
            OrientedPoint* orientedPoint,
            glm::vec3 scale = glm::vec3(1.f))
    {
        this->centerPoint = orientedPoint;
        this->scale = scale;

        this->nrOfVertices = primitive->getNrOfVertices();
        this->nrOfIndices = primitive->getNrOfIndices();

        this->vertexArray = new Vertex[this->nrOfVertices];
        for (size_t i = 0; i < this->nrOfVertices; i++)
        {
            this->vertexArray[i] = primitive->getVertices()[i];
        }

        this->indexArray = new GLuint[this->nrOfIndices];
        for (size_t i = 0; i < this->nrOfIndices; i++)
        {
            this->indexArray[i] = primitive->getIndices()[i];
        }

        this->updateModelMatrix();
    }

    Mesh(const Mesh& obj)
    {
        this->centerPoint = obj.centerPoint;
        this->scale = obj.scale;

        this->nrOfVertices = obj.nrOfVertices;
        this->nrOfIndices = obj.nrOfIndices;

        this->vertexArray = new Vertex[this->nrOfVertices];
        for (size_t i = 0; i < this->nrOfVertices; i++)
        {
            this->vertexArray[i] = obj.vertexArray[i];
        }

        this->indexArray = new GLuint[this->nrOfIndices];
        for (size_t i = 0; i < this->nrOfIndices; i++)
        {
            this->indexArray[i] = obj.indexArray[i];
        }

        this->updateModelMatrix();
    }

    ~Mesh()
    {
        delete[] this->vertexArray;
        delete[] this->indexArray;
    }

    void setScale(const glm::vec3 setScale)
    {
        this->scale = scale;
    }

    void scaleUp(const glm::vec3 scale)
    {
        this->scale += scale;
    }
};

#endif //SWTOR_MESH_H
