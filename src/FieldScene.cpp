#include "FieldScene.h"
#include "Engine/OBJLoader.h"
#include "Engine/AssimpLoader.h"
#include "Physics/rigidbody.h"
#include "Animations/BasicAnimator.h"

FieldScene::FieldScene(int GL_VERSION_MAJOR, int GL_VERSION_MINOR, int framebufferWidth, int framebufferHeight)
{
    this->initialize(GL_VERSION_MAJOR, GL_VERSION_MINOR, framebufferWidth, framebufferHeight);
}

void FieldScene::initShaders(int GL_VERSION_MAJOR, int GL_VERSION_MINOR)
{
    this->shaders.push_back(new Shader(GL_VERSION_MAJOR, GL_VERSION_MINOR,
                                       "Shaders/vertex_core.glsl", "Shaders/fragment_directional.glsl"));
    this->shaders.push_back(new Shader(GL_VERSION_MAJOR, GL_VERSION_MINOR,
                                       "Shaders/vertex_unlit.glsl", "Shaders/fragment_unlit.glsl"));
    this->shaders.push_back(new Shader(GL_VERSION_MAJOR, GL_VERSION_MINOR,
                                       "Shaders/vertex_skinned_diffuse.glsl", "Shaders/fragment_skinned_diffuse.glsl"));
}

void FieldScene::initTextures()
{
    this->textures.push_back(new Texture("Images/skybox.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("Images/red.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("Images/red.png", GL_TEXTURE_2D));
}

void FieldScene::initMaterials()
{
    this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
}
std::vector<AnimationClip> clips;//<--MUST be deleted
void FieldScene::initObjects()
{/*
    GameObject* skybox = new GameObject(glm::vec3(2.f, 0.f, 0.f), glm::vec3(180.f, 0.f, 0.f));
    skybox->addComponent<Model>();
    std::vector<Vertex> mesh = loadOBJ("OBJFiles/skybox.obj");
    skybox->getComponent<Model>()->addMesh(mesh, this->materials[0], this->shaders[1], this->textures[0], this->textures[0]);
    this->gameObjects.push_back(skybox);
*/
    //std::vector<AnimationClip> clips;
    auto holocroneData = AssimpLoader::loadWithArmature("OBJFiles/Strut Walking.fbx", clips);  
    GameObject* character = new GameObject(glm::vec3(3.f, -15.f, 0.f), glm::vec3(0.f, -90.f,0.f), glm::vec3(0.1f));
    character->addComponent<Model>();
    SkinnedMesh* holoMesh = new SkinnedMesh(holocroneData[0].vertices.data(), holocroneData[0].vertices.size(), holocroneData[0].indices.data(), holocroneData[0].indices.size(), character, holocroneData[0].boneNameToIndexMap);
    SkinnedMesh* secondMesh = new SkinnedMesh(holocroneData[1].vertices.data(), holocroneData[1].vertices.size(), holocroneData[1].indices.data(), holocroneData[1].indices.size(), character, holocroneData[1].boneNameToIndexMap);
    character->getComponent<Model>()->addSkinnedMesh(holoMesh, this->materials[0], this->shaders[2], this->textures[1], this->textures[2]);
    character->getComponent<Model>()->addSkinnedMesh(secondMesh, this->materials[0], this->shaders[2], this->textures[1], this->textures[2]);
    
    character->addComponent<BasicAnimator>();
    character->getComponent<BasicAnimator>()->setupStateMachine(&clips[0]);
    character->getComponent<BasicAnimator>()->attachMesh(holoMesh);
    character->getComponent<BasicAnimator>()->attachMesh(secondMesh);
    this->gameObjects.push_back(character);

    GameObject* camera = new GameObject(glm::vec3(-50.f, 0.f, 0.f), glm::vec3(0.f, -90.f, 0.f));
    camera->addComponent<Camera>();
    this->renderCamera = camera->getComponent<Camera>();
    this->gameObjects.push_back(camera);

    GameObject* direcionalLight = new GameObject(glm::vec3(-50.f, 50.f, 50.f), glm::vec3(0.f, 0.f, 0.f));
    direcionalLight->addComponent<PointLight>();
    this->pointLights.push_back(direcionalLight->getComponent<PointLight>());
    this->gameObjects.push_back(direcionalLight);
}