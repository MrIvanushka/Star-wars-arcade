//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_MODEL_H
#define SWTOR_MODEL_H

#include"../Engine/Mesh.h"
#include"../Engine/Texture.h"
#include"../Engine/Shader.h"
#include"../Engine/Material.h"
#include "../Engine/GameObject.h"
#include "../Engine/MeshRenderer.h"
#include "../Engine/SkinnedMeshRenderer.h"

class Model : public Component
{
private:
    std::vector<IMeshRenderer*> meshes;
public:
    Model(GameObject* object) : Component(object)
    {}

    void addMesh(
            std::vector<Vertex> mesh,
            Material* material,
            Shader* shader,
            Texture* orTexDif,
            Texture* orTexSpec
    ) {
            this->meshes.push_back(
                    new MeshRenderer(new Mesh(mesh.data(), mesh.size(), NULL, 0, gameObject),
                                     material, shader, orTexDif, orTexSpec));
    }

    void addMesh(
            Mesh* mesh,
            Material* material,
            Shader* shader,
            Texture* orTexDif,
            Texture* orTexSpec
    ) {
            //Need to set center point to mesh
            this->meshes.push_back(
                    new MeshRenderer(mesh,
                                     material, shader, orTexDif, orTexSpec));
    }

    void addSkinnedMesh(SkinnedMesh* mesh, Material* material, Shader* shader, Texture* orTexDif, Texture* orTexSpec) {
        this->meshes.push_back(new SkinnedMeshRenderer(mesh, material, shader, orTexDif, orTexSpec));
    }

    ~Model()
    {
        for (auto*& i : this->meshes)
            delete i;
    }

    void render() override
    {
        for (IMeshRenderer* renderer : meshes)
        {
            renderer->render();
        }
    }
};

#endif //SWTOR_MODEL_H
