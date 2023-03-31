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
    aiNode* root;
    std::map<std::string, uint> boneNameToIndexMap;
    std::vector<glm::mat4> boneTransforms;
public:
    friend class SkinnedMeshRenderer;

    SkinnedMesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, OrientedPoint* orientedPoint, aiNode* root, std::map<std::string,uint> boneNameToIndexMap, std::vector<glm::mat4> boneTransforms) :
        Mesh(vertexArray, nrOfVertices, indexArray, nrOfIndices, orientedPoint), root(root), boneNameToIndexMap(boneNameToIndexMap) , boneTransforms(boneTransforms) {}

    SkinnedMesh(const SkinnedMesh& obj) : Mesh(obj), root(root) {}

    bool tryFindBoneID(std::string name, uint& id) {
        if(boneNameToIndexMap.count(name))
        {
            id = boneNameToIndexMap[name];
            return true;
        }
        return false;
    }

    aiNode* getRoot() {
        return root;
    }

    void setBoneTransforms(std::vector<glm::mat4> &transforms)
    {
        if(transforms.size() != boneTransforms.size())
            throw std::logic_error("different bone transform matrices");

        boneTransforms = transforms;
    }

    std::vector<glm::mat4> getBoneTransforms(){
        return boneTransforms;
    }
};

#endif //SWTOR_SKINNEDMESH_H
