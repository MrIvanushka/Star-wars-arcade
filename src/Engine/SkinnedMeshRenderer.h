#ifndef SWTOR_SKINNEDMESHRENDERER_H
#define SWTOR_SKINNEDMESHRENDERER_H

#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"SkinnedMesh.h"

class SkinnedMeshRenderer
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint BONE_BUFFER;
    GLuint EBO;
    SkinnedMesh* mesh;
    Material* material;
    Shader* shader;
    Texture* overrideTextureDiffuse;
    Texture* overrideTextureSpecular;

    void initVAO();

    void updateUniforms(Shader* shader);
public:
    SkinnedMeshRenderer(SkinnedMesh* mesh, Material* material, Shader* shader, Texture* diffuse, Texture* specular);

    ~SkinnedMeshRenderer();

    void render();
};

#endif //SWTOR_SKINNEDMESHRENDERER_H
