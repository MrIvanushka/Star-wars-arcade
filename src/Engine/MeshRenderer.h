//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_MESHRENDERER_H
#define SWTOR_MESHRENDERER_H

#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"Mesh.h"

class MeshRenderer
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    Mesh* mesh;
    Material* material;
    Shader* shader;
    Texture* overrideTextureDiffuse;
    Texture* overrideTextureSpecular;

    void initVAO()
    {
        //Create VAO
        glCreateVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        //GEN VBO AND BIND AND SEND DATA
        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->mesh->nrOfVertices * sizeof(Vertex), this->mesh->vertexArray, GL_STATIC_DRAW);

        //GEN EBO AND BIND AND SEND DATA
        if (this->mesh->nrOfIndices > 0)
        {
            glGenBuffers(1, &this->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->nrOfIndices * sizeof(GLuint), this->mesh->indexArray, GL_STATIC_DRAW);
        }

        //SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
        //Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        //Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        //Texcoord
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
        glEnableVertexAttribArray(2);
        //Normal
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3);

        //BIND VAO 0
        glBindVertexArray(0);
    }

    void updateUniforms(Shader* shader)
    {
        shader->setMat4fv(this->mesh->ModelMatrix, "ModelMatrix");
    }
public:
    MeshRenderer(Mesh* mesh, Material* material, Shader* shader, Texture* diffuse, Texture* specular)
    {
        this->mesh = mesh;
        this->material = material;
        this->shader = shader;
        this->overrideTextureDiffuse = diffuse;
        this->overrideTextureSpecular = specular;
        this->initVAO();
    }

    ~MeshRenderer()
    {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);

        if (this->mesh->nrOfIndices > 0)
        {
            glDeleteBuffers(1, &this->EBO);
        }

    }

    void render()
    {
        this->material->sendToShader(*shader);
        shader->use();
        this->overrideTextureDiffuse->bind(0);
        this->overrideTextureSpecular->bind(1);

        //Update uniforms
        this->mesh->updateModelMatrix();
        this->updateUniforms(shader);

        shader->use();

        //Bind VAO
        glBindVertexArray(this->VAO);

        //RENDER
        if (this->mesh->nrOfIndices == 0)
            glDrawArrays(GL_TRIANGLES, 0, this->mesh->nrOfVertices);
        else
            glDrawElements(GL_TRIANGLES, this->mesh->nrOfIndices, GL_UNSIGNED_INT, 0);

        //Cleanup
        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

#endif //SWTOR_MESHRENDERER_H
