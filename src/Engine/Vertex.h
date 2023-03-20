//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_VERTEX_H
#define SWTOR_VERTEX_H

#include<../../glm/glm/glm.hpp>
#include<vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
    glm::vec3 tangent;
    
    static std::vector<Vertex> generateList(float* vertices, int numberOfVertices);
    static void calculateTangents(std::vector<Vertex>& list, std::vector<unsigned int>& indices);
};

#endif //SWTOR_VERTEX_H
