#ifndef SWTOR_SKINNEDMESHRENDERER_H
#define SWTOR_SKINNEDMESHRENDERER_H

#include<string>

#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"SkinnedMesh.h"
#include"MeshRenderer.h"

class SkinnedMeshRenderer : public IMeshRenderer
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    SkinnedMesh* mesh;
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
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
        //glEnableVertexAttribArray(1);
        //Texcoord
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
        glEnableVertexAttribArray(2);
        //Normal
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3);

        glVertexAttribIPointer(4, MAX_NUM_BONES_PER_VERTEX, GL_INT, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, boneIds));
        glEnableVertexAttribArray(4);
    
        glVertexAttribPointer(5, MAX_NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const GLvoid*)(offsetof(Vertex, boneWeights)));
        glEnableVertexAttribArray(5);

        //BIND VAO 0
        glBindVertexArray(0);
    }

    void updateUniforms(Shader* shader)
    {
        shader->use();
        shader->setMat4fvRaw(this->mesh->ModelMatrix, "ModelMatrix");

        for (unsigned int i = 0 ; i < this->mesh->boneTransforms.size(); i++) 
        {
            std::string name = "gBones[" + std::to_string(i) + "]";
            shader->setMat4fvRaw(this->mesh->boneTransforms[i], name.c_str());
        }
        shader->unuse();
    }
public:
    SkinnedMeshRenderer(SkinnedMesh* mesh, Material* material, Shader* shader, Texture* diffuse, Texture* specular)
    {
        this->mesh = mesh;
        this->material = material;
        this->shader = shader;
        this->overrideTextureDiffuse = diffuse;
        this->overrideTextureSpecular = specular;
        this->initVAO();
    }

    ~SkinnedMeshRenderer()
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


#endif //SWTOR_SKINNEDMESHRENDERER_H
