//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_VERTEX_H
#define SWTOR_VERTEX_H

#include<../../glm/glm/glm.hpp>
#include<vector>
#include<string>

#define MAX_NUM_BONES_PER_VERTEX 4

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
    glm::vec3 tangent;
    unsigned int boneIds[MAX_NUM_BONES_PER_VERTEX];
    float boneWeights[MAX_NUM_BONES_PER_VERTEX];

    Vertex()
    {
        for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++)
        {   
            boneIds[i] = 0;
            boneWeights[i] = 0;
        }
    }

    static std::vector<Vertex> generateList(float* vertices, int numberOfVertices);
    static void calculateTangents(std::vector<Vertex>& list, std::vector<unsigned int>& indices);
};

#endif //SWTOR_VERTEX_H
