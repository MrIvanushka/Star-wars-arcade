#include "Vertex.h"

std::vector<Vertex> Vertex::generateList(float* vertices, int noVertices) {
    std::vector<Vertex> ret(noVertices);

    int stride = 8;

    for (int i = 0; i < noVertices; i++) {
        ret[i].position = glm::vec3(
            vertices[i * stride + 0],
            vertices[i * stride + 1],
            vertices[i * stride + 2]
        );

        ret[i].normal = glm::vec3(
            vertices[i * stride + 3],
            vertices[i * stride + 4],
            vertices[i * stride + 5]
        );

        ret[i].texcoord = glm::vec2(
            vertices[i * stride + 6],
            vertices[i * stride + 7]
        );
    }

    return ret;
}

void averageVectors(glm::vec3& baseVec, glm::vec3 addition, unsigned char existingContributions) {
    if (!existingContributions) {
        baseVec = addition;
    }
    else {
        float f = 1 / ((float)existingContributions + 1);

        baseVec *= (float)(existingContributions)*f;

        baseVec += addition * f;
    }
}

void Vertex::calculateTangents(std::vector<Vertex>& list, std::vector<unsigned int>& indices) {
    unsigned char* counts = (unsigned char*)malloc(list.size() * sizeof(unsigned char));
    for (unsigned int i = 0, len = list.size(); i < len; i++) {
        counts[i] = 0;
    }

    for (unsigned int i = 0, len = indices.size(); i < len; i += 3) {
        Vertex v1 = list[indices[i + 0]];
        Vertex v2 = list[indices[i + 1]];
        Vertex v3 = list[indices[i + 2]];

        glm::vec3 edge1 = v2.position - v1.position;
        glm::vec3 edge2 = v3.position - v1.position;

        // calculate dUVs
        glm::vec2 deltaUV1 = v2.texcoord - v1.texcoord;
        glm::vec2 deltaUV2 = v3.texcoord - v1.texcoord;

        // use inverse of the UV matrix to determine tangent
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

        glm::vec3 tangent = {
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        };

        // average in the new tangent vector
        averageVectors(list[indices[i + 0]].tangent, tangent, counts[indices[i + 0]]++);
        averageVectors(list[indices[i + 1]].tangent, tangent, counts[indices[i + 1]]++);
        averageVectors(list[indices[i + 2]].tangent, tangent, counts[indices[i + 2]]++);
    }
}