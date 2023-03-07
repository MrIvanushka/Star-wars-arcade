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

class Model : public Component
{
private:
    std::vector<MeshRenderer*> meshes;
public:
    Model(GameObject* object) : Component(object)
    {}

    void addMesh(
            std::vector<Vertex> mesh,
            float scale,
            Material* material,
            Shader* shader,
            Texture* orTexDif,
            Texture* orTexSpec
    ) {
            this->meshes.push_back(
                    new MeshRenderer(new Mesh(mesh.data(), mesh.size(), NULL, 0, gameObject, glm::vec3(scale)),
                                     material, shader, orTexDif, orTexSpec));
    }

    ~Model()
    {
        for (auto*& i : this->meshes)
            delete i;
    }

    void render() override
    {
        for (MeshRenderer* renderer : meshes)
        {
            renderer->render();
        }
    }
};

#endif //SWTOR_MODEL_H
