#ifndef SWTOR_NAVMESHGENERATOR_H
#define SWTOR_NAVMESHGENERATOR_H

#include<set>
#include<map>

#include "../Physics/linalg.h"
#include "../Engine/Vertex.h"
#include "NavMesh.hpp"


class NavMeshGenerator{
public:
    static NavMeshSurface* generate(std::vector<Vertex> vertices, std::vector<unsigned int> indices, OrientedPoint* center){
        glm::mat4 model = center->getModelMatrix();
        std::vector<bool> checked(vertices.size());
        std::vector<glm::vec3> points(vertices.size());
        std::vector<unsigned int> inds;
        float maxDist = 0;
        int checkingIndex = 0;
        glm::vec3 zero(0.f);

        for (int i = 0; i < indices.size() / 3; i++)
	    {
		    if(indices[3*i] != indices[3*i+1] && indices[3*i] != indices[3*i+2] && indices[3*i+1] != indices[3*i+2])
		    {
			    inds.push_back(indices[3*i]);
			    inds.push_back(indices[3*i + 1]);
			    inds.push_back(indices[3*i + 2]);
		    }
	    }
        for(int i = 0; i < vertices.size(); i++)
        {
            checked[i] = false;
            float dist = glm::distance(zero, vertices[i].position);

            if(dist > maxDist)
            {
                maxDist = dist;
                checkingIndex = i;
            }
            points[i] = mat4vec3mult(model, vertices[i].position);
        }
        std::vector<TPPLPoint> cornerPoints = findPolygon(checkingIndex, points, inds, checked);
        std::cout << "CORNER " << cornerPoints.size() << std::endl;

        for(int i = 0; i < cornerPoints.size(); i++)
            std::cout << cornerPoints[i].x << " " << cornerPoints[i].y << " " << std::endl;

        NavMeshSurface* surface = new NavMeshSurface(cornerPoints);
        
        for(int i = 0; i < vertices.size(); i++)
        {
            if(checked[i] == false)
            {
                cornerPoints = findPolygon(i, points, inds, checked);
                 std::cout << "CORNER " << cornerPoints.size() << std::endl;

                if(cornerPoints.size() > 2)
                {
                    for(int i = 0; i < cornerPoints.size(); i++)
                        std::cout << cornerPoints[i].x << ";" << cornerPoints[i].y << ";" << std::endl;
                    surface->add_area(cornerPoints);
                }
            }
        }

        return surface;
    }
private:
    static std::vector<TPPLPoint> findPolygon(unsigned int checkIndex, std::vector<glm::vec3> &points, std::vector<unsigned int> &indices, std::vector<bool> &checked)
    {
        std::vector<TPPLPoint> foundPolygons;

        for(int i = 0; i < points.size(); i++){
            auto checkPoint = points[checkIndex];

            for(int i = 0; i < indices.size(); i++)
                if(glm::distance(points[indices[i]], checkPoint) < 0.0001f)
                    checked[indices[i]] = true;
            
            foundPolygons.push_back(TPPLPoint(points[checkIndex].x, points[checkIndex].z));

            std::set<unsigned int> neighbours = findNeighbours(checkIndex, points, indices, checked);

            if(neighbours.size() == 0)
                break;
            else{
                for(auto& index : neighbours){
                    checkIndex = index;
                }
            }
        }

        return foundPolygons;
    }
    
    static std::set<unsigned int> findNeighbours(unsigned int index, std::vector<glm::vec3> &points, std::vector<unsigned int> &indices, std::vector<bool> &checked){
        std::map<float, unsigned int> neighboursRaw;
        std::set<unsigned int> neighbours;

        for(int i = 0; i < indices.size() / 3; i++)
        {
            if(samePointsCount({points[index]},{points[indices[3*i]], points[indices[3*i+1]], points[indices[3*i+2]]})>0){

                for(int j = 0; j < 3; j++){
                    auto hash = hashValue(points[indices[3*i+j]]);
                    
                    if(neighboursRaw.contains(hash))
                    {
                        unsigned int idx = neighboursRaw[hash];
                        if(neighbours.contains(idx))
                        {
                            neighbours.erase(idx);
                        }
                    }
                    else if(!checked[indices[3*i+j]])
                    {
                        neighboursRaw[hash] = indices[3*i+j];
                        neighbours.insert(indices[3*i+j]);
                    }
                }
            }
        }

        return neighbours;
    }
    static float hashValue(glm::vec3 a){
        return a.x + 100*a.y + 1000*a.z;
    }

    static int samePointsCount(std::vector<glm::vec3> &&first, std::vector<glm::vec3> &&second){
        unsigned int count = 0;

        for(int i = 0; i < first.size(); i++)
            for(int j = 0; j < second.size(); j++)
                if(first[i] == second[j])
                    count++;

        return count;
    }
};

#endif //SWTOR_TERRAINGENERATOR_H