#include "SkinnedMeshRenderer.h"

SkinnedMeshRenderer::SkinnedMeshRenderer(SkinnedMesh* mesh, Material* material, Shader* shader, Texture* diffuse, Texture* specular)
{
    this->mesh = mesh;
    this->material = material;
    this->shader = shader;
    this->overrideTextureDiffuse = diffuse;
    this->overrideTextureSpecular = specular;
    this->initVAO();
}

void SkinnedMeshRenderer::initVAO()
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
    glEnableVertexAttribArray(1);
    //Normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);/*
    //Bones
    //glBindBuffer(GL_ARRAY_BUFFER, BONE_BUFFER);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(m_Bones[0]) * m_Bones.size(), &m_Bones[0], GL_STATIC_DRAW);
    glVertexAttribIPointer(4, MAX_NUM_BONES_PER_VERTEX, GL_INT, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, bones));
    glEnableVertexAttribArray(4);
    
    glVertexAttribPointer(5, MAX_NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const GLvoid*)(offsetof(Vertex, bones) + MAX_NUM_BONES_PER_VERTEX * sizeof(int32_t)));
    glEnableVertexAttribArray(5);*/

    //BIND VAO 0
    glBindVertexArray(0);
}

void SkinnedMeshRenderer::updateUniforms(Shader* shader)
{
    shader->setMat4fv(this->mesh->ModelMatrix, "ModelMatrix");
/*
    for (unsigned int i = 0 ; i < this->mesh->boneTransforms.size(); i++) {
        shader->setMat4fv(this->mesh->boneTransforms[i], "gBones[%d]", i);
    }*/
}

void SkinnedMeshRenderer::render()
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

