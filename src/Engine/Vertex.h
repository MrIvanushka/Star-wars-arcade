//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_VERTEX_H
#define SWTOR_VERTEX_H

#include<../../glm/glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
    glm::vec3 normal;
};

#endif //SWTOR_VERTEX_H
