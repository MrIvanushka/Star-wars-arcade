#ifndef SWTOR_SKINNEDMESH_H
#define SWTOR_SKINNEDMESH_H

#include<iostream>
#include<vector>
#include<map>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <../../glm/glm/glm.hpp>
#include <../../glm/glm/gtx/matrix_decompose.hpp>
#include <../../glm/glm/gtx/matrix_interpolation.hpp>

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

    void setBoneTransforms(std::vector<glm::mat4> transforms, float blendFactor)
    {
        if(blendFactor > 1)
            blendFactor = 1;

        for(int i = 0; i < transforms.size(); i++)
        {
            glm::vec3 firstScale;
		    glm::quat firstRotation;
		    glm::vec3 firstTranslation;
		    glm::vec3 secondScale;
		    glm::quat secondRotation;
		    glm::vec3 secondTranslation;
		    glm::vec3 skew;
		    glm::vec4 perspective;
		    glm::decompose(boneTransforms[i], firstScale, firstRotation, firstTranslation, skew, perspective);
		    glm::decompose(transforms[i], secondScale, secondRotation, secondTranslation, skew, perspective);

		glm::mat4 translationM = glm::translate(glm::mat4(1.f), firstTranslation + blendFactor*(secondTranslation - firstTranslation));
		glm::mat4 scaleM = glm::scale(glm::mat4(1.f), firstScale + blendFactor*(secondScale - firstScale));
		glm::mat4 rotationM = glm::interpolate(glm::mat4_cast(firstRotation), glm::mat4_cast(secondRotation), blendFactor);
		transforms[i] = translationM * rotationM * scaleM;
        }

        boneTransforms = transforms;
    }
};

#endif //SWTOR_SKINNEDMESH_H
