#include "FieldScene.h"
#include "Engine/OBJLoader.h"
#include "Engine/AssimpLoader.h"
#include "Physics/rigidbody.h"
#include "Animations/BasicBlendAnimator.h"
#include "Animations/BasicAnimator.h"
#include"GameComponents/CameraController.h"
#include"GameComponents/CameraFollower.h"
#include"GameComponents/Follower.h"
#include"GameComponents/PlayerMovement.h"

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

Assimp::Importer importer1;
Assimp::Importer importer2;
std::vector<AnimationClip> clips;//<--MUST be deleted
void FieldScene::initObjects()
{/*
    GameObject* skybox = new GameObject(glm::vec3(2.f, 0.f, 0.f), glm::vec3(180.f, 0.f, 0.f));
    skybox->addComponent<Model>();
    std::vector<Vertex> mesh = loadOBJ("OBJFiles/skybox.obj");
    skybox->getComponent<Model>()->addMesh(mesh, this->materials[0], this->shaders[1], this->textures[0], this->textures[0]);
    this->gameObjects.push_back(skybox);
*/
    auto cubeData = AssimpLoader::loadWithArmature(importer1, "OBJFiles/Cube.fbx", clips);
    GameObject* cube = new GameObject(glm::vec3(3.f, -6.f, 0.f), glm::vec3(0.f), glm::vec3(2.f,9.f,2.f));
    //cube->addComponent<Model>();
    //Mesh* cubeMesh = new Mesh(cubeData[0].vertices.data(), cubeData[0].vertices.size(), cubeData[0].indices.data(), cubeData[0].indices.size(), cube);
    //cube->getComponent<Model>()->addMesh(cubeMesh, this->materials[0], this->shaders[0], this->textures[1], this->textures[2]);
    cube->addComponent<MeshCollider>();
    cube->getComponent<MeshCollider>()->initialize(cubeData[0].vertices, cubeData[0].indices);
    cubeData[0].br.collider = cube->getComponent<MeshCollider>();
    collisionProcessor->addToPending(cube, &cubeData[0].br);
    this->gameObjects.push_back(cube);

    GameObject* platform = new GameObject(glm::vec3(3.f, -60.f, 0.f), glm::vec3(20.f, 0.f, 0.f), glm::vec3(20.f,10.f,20.f));
    platform->addComponent<Model>();
    Mesh* platformMesh = new Mesh(cubeData[0].vertices.data(), cubeData[0].vertices.size(), cubeData[0].indices.data(), cubeData[0].indices.size(), platform);
    platform->getComponent<Model>()->addMesh(platformMesh, this->materials[0], this->shaders[0], this->textures[1], this->textures[2]);
    platform->addComponent<MeshCollider>();
    platform->getComponent<MeshCollider>()->initialize(cubeData[0].vertices, cubeData[0].indices);
    cubeData[0].br.collider = platform->getComponent<MeshCollider>();
    collisionProcessor->addToPending(platform, &cubeData[0].br);
    this->gameObjects.push_back(platform);

    GameObject* platform2 = new GameObject(glm::vec3(3.f, -60.f, 10.f), glm::vec3(-20.f, 0.f, 0.f), glm::vec3(20.f,10.f,20.f));
    platform2->addComponent<Model>();
    Mesh* platform2Mesh = new Mesh(cubeData[0].vertices.data(), cubeData[0].vertices.size(), cubeData[0].indices.data(), cubeData[0].indices.size(), platform2);
    platform2->getComponent<Model>()->addMesh(platform2Mesh, this->materials[0], this->shaders[0], this->textures[1], this->textures[2]);
    platform2->addComponent<MeshCollider>();
    platform2->getComponent<MeshCollider>()->initialize(cubeData[0].vertices, cubeData[0].indices);
    cubeData[0].br.collider = platform2->getComponent<MeshCollider>();
    collisionProcessor->addToPending(platform2, &cubeData[0].br);
    this->gameObjects.push_back(platform2);

    GameObject* platform3 = new GameObject(glm::vec3(13.f, -60.f, 5.f), glm::vec3(20.f, 90.f, 0.f), glm::vec3(20.f,10.f,20.f));
    platform3->addComponent<Model>();
    Mesh* platform3Mesh = new Mesh(cubeData[0].vertices.data(), cubeData[0].vertices.size(), cubeData[0].indices.data(), cubeData[0].indices.size(), platform3);
    platform3->getComponent<Model>()->addMesh(platform3Mesh, this->materials[0], this->shaders[0], this->textures[1], this->textures[2]);
    platform3->addComponent<MeshCollider>();
    platform3->getComponent<MeshCollider>()->initialize(cubeData[0].vertices, cubeData[0].indices);
    cubeData[0].br.collider = platform3->getComponent<MeshCollider>();
    collisionProcessor->addToPending(platform3, &cubeData[0].br);
    this->gameObjects.push_back(platform3);
    
    auto holocroneData = AssimpLoader::loadWithArmature(importer1, "OBJFiles/Idle.fbx", clips);  
    auto otherData = AssimpLoader::loadWithArmature(importer2, "OBJFiles/Walking.fbx", clips);  
    GameObject* character = new GameObject(glm::vec3(3.f, -15.f, 0.f), glm::vec3(0.f), glm::vec3(0.1f));
    character->addComponent<Model>();
    SkinnedMesh* holoMesh = new SkinnedMesh(holocroneData[0].vertices.data(), holocroneData[0].vertices.size(), holocroneData[0].indices.data(), holocroneData[0].indices.size(), character, holocroneData[0].boneNameToIndexMap);
    SkinnedMesh* secondMesh = new SkinnedMesh(holocroneData[1].vertices.data(), holocroneData[1].vertices.size(), holocroneData[1].indices.data(), holocroneData[1].indices.size(), character, holocroneData[1].boneNameToIndexMap);
    character->getComponent<Model>()->addSkinnedMesh(holoMesh, this->materials[0], this->shaders[2], this->textures[1], this->textures[2]);
    character->getComponent<Model>()->addSkinnedMesh(secondMesh, this->materials[0], this->shaders[2], this->textures[1], this->textures[2]);
    cube->addComponent<PlayerMovement>();
    cube->addComponent<CharacterController>();
    character->addComponent<Follower>();
    character->getComponent<Follower>()->setTarget(cube);

    character->addComponent<BasicBlendAnimator>();
    character->getComponent<BasicBlendAnimator>()->setupStateMachine(clips, cube->getComponent<CharacterController>());
    character->getComponent<BasicBlendAnimator>()->attachMesh(holoMesh);
    character->getComponent<BasicBlendAnimator>()->attachMesh(secondMesh);

    GameObject* camera = new GameObject(glm::vec3(-25.f, 0.f, 0.f), glm::vec3(0.f, -90.f, 0.f));
    camera->addComponent<CameraController>();
    camera->addComponent<CameraFollower>();
    camera->getComponent<CameraFollower>()->setTarget(cube);
    camera->addComponent<Camera>();
    this->renderCamera = camera->getComponent<Camera>();
    this->gameObjects.push_back(camera);
    this->gameObjects.push_back(character);

    cube->getComponent<PlayerMovement>()->attachCamera(camera->getComponent<Camera>());

    GameObject* direcionalLight = new GameObject(glm::vec3(-50.f, 50.f, 50.f), glm::vec3(0.f, 0.f, 0.f));
    direcionalLight->addComponent<PointLight>();
    this->pointLights.push_back(direcionalLight->getComponent<PointLight>());
    this->gameObjects.push_back(direcionalLight);
}