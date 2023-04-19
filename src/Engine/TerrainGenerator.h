#ifndef SWTOR_TERRAINGENERATOR_H
#define SWTOR_TERRAINGENERATOR_H

#include "Vertex.h"
#include "AssimpLoader.h"
#include<cmath>

class TerrainGenerator{
public:
    static std::vector<MeshPack> processTerrain(std::vector<Vertex> &vertices){
        glm::vec2 center(0);
        glm::vec2 max(-100000, -100000);
        glm::vec2 min(100000, 100000);
        
        for(int i = 0; i < vertices.size(); i++)
        {
            if(min.x > vertices[i].position.x)
                min.x = vertices[i].position.x;
            if(min.y > vertices[i].position.y)
                min.y = vertices[i].position.y;
            if(max.x < vertices[i].position.x)
                max.x = vertices[i].position.x;
            if(max.y < vertices[i].position.y)
                max.y = vertices[i].position.y;
        }
        center = (min + max) * 0.5f;
        int terrainSizeX = 160;
        int terrainSizeY = (int)(terrainSizeX * (max.y-min.y)/(max.x-min.x));
        float cellSize = (max.x-min.x) / terrainSizeX;
        std::vector<std::vector<float>> heights(terrainSizeX + 1);

        for(int i = 0; i < terrainSizeX+1; i++)
        {
            heights[i] = std::vector<float>(terrainSizeY + 1);

            for(int j = 0; j < terrainSizeY; j++)
                heights[i][j] = -1000;
        }

        for(int i = 0; i < vertices.size(); i++)
        {
            glm::vec2 offset(vertices[i].position.x - min.x, vertices[i].position.y - min.y);
            int coordX = (int)round(offset.x / cellSize);
            int coordY = (int)round(offset.y / cellSize);
            heights[coordX][coordY] = vertices[i].position.z;
            vertices[i].position.x = coordX * cellSize + min.x;
            vertices[i].position.y = coordY * cellSize + min.y;

            vertices[i].texcoord.x = (center.x + vertices[i].position.x) * 20 / (max.x - min.x);
            vertices[i].texcoord.y = (center.y + vertices[i].position.y) * 20 / (max.y - min.y);
        }
        
        for(int i = 1; i < terrainSizeX; i++)
            for(int j = 1; j < terrainSizeX; j++)
                if(heights[i][j] < -500)
                {
                    if(heights[i+1][j] > -500 && heights[i-1][j] > -500)
                        heights[i][j] = (heights[i+1][j] + heights[i-1][j]) / 2;
                    if(heights[i][j+1] > -500 && heights[i][j-1] > -500)
                        heights[i][j] = (heights[i][j+1] + heights[i][j-1]) / 2;
                }
        for(int i = 5; i < terrainSizeX - 4; i++)
            for(int j = 5; j < terrainSizeX - 4; j++)
                if(heights[i][j] < -500)
                {
                    if(heights[i+5][j] > -500 && heights[i-5][j] > -500)
                        heights[i][j] = (heights[i+5][j] + heights[i-5][j]) / 2;
                    if(heights[i][j+5] > -500 && heights[i][j-5] > -500)
                        heights[i][j] = (heights[i][j+5] + heights[i][j-5]) / 2;
                }

        std::vector<MeshPack> ret;
        std::vector<unsigned int> indices;

        int cellSizeX = terrainSizeX / 20;
        int cellSizeY = terrainSizeY / 20;

        for(int i = 0; i < cellSizeX; i++)
        {
            for(int j = 0; j < cellSizeY; j++)
            {
                indices.push_back((cellSizeY+1)*i+j);
                indices.push_back((cellSizeY+1)*(i+1)+j+1);
                indices.push_back((cellSizeY+1)*i+j+1);
                indices.push_back((cellSizeY+1)*(i+1)+j);
                indices.push_back((cellSizeY+1)*(i+1)+j+1);
                indices.push_back((cellSizeY+1)*i+j);
            }
        }

        for(int x = 1; x < 20; x++)
        {
            for(int y = 1; y < 20; y++)
            {
                MeshPack newMeshPack;
                newMeshPack.vertices = std::vector<Vertex>();
                newMeshPack.indices = indices;

                for(int i = cellSizeX * x - 1; i < cellSizeX * x + cellSizeX; i++)
                {
                    for(int j = cellSizeY * y - 1; j < cellSizeY * y + cellSizeY; j++)
                    {
                        Vertex vertex;
                        vertex.normal = glm::vec3(0,1,0);
                        vertex.texcoord = glm::vec2(1);
                        vertex.position = glm::vec3(i * cellSize + min.x, j * cellSize + min.y, heights[i][j]);
                        newMeshPack.vertices.push_back(vertex);
                    }
                }
                ret.push_back(newMeshPack);
            }
        }
        return ret;
    }
};

#endif //SWTOR_TERRAINGENERATOR_H
