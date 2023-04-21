#ifndef SWTOR_FIELDSCENE_H
#define SWTOR_FIELDSCENE_H

#include "Engine/Scene.h"
#include "Engine/AssimpLoader.h"
#include"Navigation/NavMeshGenerator.h"

class FieldScene : public Scene
{
private:
    void initShaders(int GL_VERSION_MAJOR, int GL_VERSION_MINOR) override;
    void initTextures() override;
    void initMaterials() override;
    void initObjects() override;
    GameObject* createCharacter(std::vector<MeshPack> &cubeData, std::vector<MeshPack> &characterData, glm::vec3 pos, Fraction fraction);
    GameObject* createJediBot(std::vector<MeshPack> &cubeData, std::vector<MeshPack> &characterData, glm::vec3 pos, NavMeshSurface* surface, Fraction fraction);
    GameObject* createSquadleaderBot(std::vector<MeshPack> &cubeData, std::vector<MeshPack> &characterData, glm::vec3 pos, NavMeshSurface* surface, Fraction fraction);
public:
    FieldScene(int GL_VERSION_MAJOR, int GL_VERSION_MINOR, int framebufferWidth, int framebufferHeight);
};

#endif //SWTOR_FIELDSCENE_H