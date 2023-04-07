#ifndef SWTOR_SKINNEDMESH_H
#define SWTOR_SKINNEDMESH_H

#include<iostream>
#include<vector>
#include<map>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <../../glm/glm/glm.hpp>

#include"Vertex.h"
#include"Mesh.h"
#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"GameObject.h"
#include"../Physics/bounds.h"

class SkinnedMesh : public Mesh 
{
private:
    std::map<std::string, uint> boneNameToIndexMap;
    std::vector<glm::mat4> boneTransforms;
public:
    friend class SkinnedMeshRenderer;

    SkinnedMesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, OrientedPoint* orientedPoint, std::map<std::string,uint> boneNameToIndexMap) :
        Mesh(vertexArray, nrOfVertices, indexArray, nrOfIndices, orientedPoint), boneNameToIndexMap(boneNameToIndexMap) {
            boneTransforms = std::vector<glm::mat4>(boneNameToIndexMap.size());
        }

    SkinnedMesh(const SkinnedMesh& obj) : Mesh(obj) {}

    bool tryFindBoneID(std::string name, uint& id) {
        if(boneNameToIndexMap.count(name))
        {
            id = boneNameToIndexMap[name];
            return true;
        }
        return false;
    }

    uint bonesCount(){
        return boneNameToIndexMap.size();
    }

    void setBoneTransforms(std::vector<glm::mat4> transforms)
    {
        if(transforms.size() != boneTransforms.size())
            throw std::logic_error("different bone transform matrices");

        boneTransforms = transforms;
    }
};

#endif //SWTOR_SKINNEDMESH_H
