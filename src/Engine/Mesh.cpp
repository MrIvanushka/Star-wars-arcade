#include "Mesh.h"

void Mesh::updateModelMatrix()
{
    this->ModelMatrix = glm::mat4(1.f);
    this->ModelMatrix = glm::translate(this->ModelMatrix, this->centerPoint->getPosition());
    glm::quat rotation = this->centerPoint->getRotation();
    this->ModelMatrix *= glm::mat4_cast(rotation);
    this->ModelMatrix = glm::scale(this->ModelMatrix, centerPoint->getScale());
}

Mesh::Mesh(
            Vertex* vertexArray,
            const unsigned& nrOfVertices,
            GLuint* indexArray,
            const unsigned& nrOfIndices,
            OrientedPoint* orientedPoint)
{
    this->centerPoint = orientedPoint;

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

Mesh::Mesh(const Mesh& obj)
{
    this->centerPoint = obj.centerPoint;

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

Mesh::~Mesh()
{
    delete[] this->vertexArray;
    delete[] this->indexArray;
}