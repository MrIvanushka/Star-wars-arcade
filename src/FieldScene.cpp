#include "FieldScene.h"
#include "Engine/OBJLoader.h"
#include "Engine/AssimpLoader.h"
#include "Physics/rigidbody.h"
#include "Animations/BasicBlendAnimator.h"
#include "Animations/BasicAnimator.h"
#include "Animations/CharacterAnimator.h"
#include"GameComponents/CameraController.h"
#include"GameComponents/CameraFollower.h"
#include"GameComponents/Follower.h"
#include"GameComponents/PlayerMovement.h"
#include"GameComponents/PlayerAttackController.h"
#include"GameComponents/DamageDealArea.h"

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
    this->textures.push_back(new Texture("Images/Walls_1.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("Images/Walls_1(Bumped).png", GL_TEXTURE_2D));
}

void FieldScene::initMaterials()
{
    this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
}

Assimp::Importer importer1;
Assimp::Importer importer2;
Assimp::Importer importer3;
Assimp::Importer importer4;
Assimp::Importer importer5;
Assimp::Importer importer6;
std::vector<AnimationClip> clips;//<--MUST be deleted

void FieldScene::initObjects()
{/*
    GameObject* skybox = new GameObject(glm::vec3(2.f, 0.f, 0.f), glm::vec3(180.f, 0.f, 0.f));
    skybox->addComponent<Model>();
    std::vector<Vertex> mesh = loadOBJ("OBJFiles/skybox.obj");
    skybox->getComponent<Model>()->addMesh(mesh, this->materials[0], this->shaders[1], this->textures[0], this->textures[0]);
    this->gameObjects.push_back(skybox);
*/

    auto data = AssimpLoader::load(importer1, "OBJFiles/temple.fbx");
    
    for(int i = 0; i < data.size(); i++)
    {
        GameObject* cube = new GameObject(glm::vec3(200.f, -160.f, 0.f), glm::vec3(-90.f, 0.f, 0.f), glm::vec3(5.f));
        cube->addComponent<Model>();
        Mesh* cubeMesh = new Mesh(data[i].vertices.data(), data[i].vertices.size(), data[i].indices.data(), data[i].indices.size(), cube);
        cube->getComponent<Model>()->addMesh(cubeMesh, this->materials[0], this->shaders[0], this->textures[3], this->textures[4]);
        this->gameObjects.push_back(cube);
    }

    auto templeData = AssimpLoader::load(importer1, "OBJFiles/templeColliders.fbx");

    for(int i = 0; i < templeData.size(); i++)
    {
        GameObject* cube = new GameObject(glm::vec3(200.f, -160.f, 0.f), glm::vec3(-90.f, 0.f, 0.f), glm::vec3(5.f));
        cube->addComponent<MeshCollider>();
        cube->getComponent<MeshCollider>()->initialize(templeData[i].vertices, templeData[i].indices);
        collisionProcessor->addToPending(cube, cube->getComponent<MeshCollider>()->getRegion());
        this->gameObjects.push_back(cube);
    }

    auto cubeData = AssimpLoader::load(importer1, "OBJFiles/Cube.fbx");
    GameObject* cube = new GameObject(glm::vec3(3.f, -6.f, 0.f), glm::vec3(0.f), glm::vec3(2.f,9.f,2.f));
    cube->addComponent<MeshCollider>();
    cube->getComponent<MeshCollider>()->initialize(cubeData[0].vertices, cubeData[0].indices);
    collisionProcessor->addToPending(cube, cube->getComponent<MeshCollider>()->getRegion());
    this->gameObjects.push_back(cube);

    
    auto holocroneData = AssimpLoader::loadWithArmature(importer1, "OBJFiles/Idle.fbx", clips);  
    AssimpLoader::loadWithArmature(importer2, "OBJFiles/Walking.fbx", clips);
    AssimpLoader::loadWithArmature(importer6, "OBJFiles/Running.fbx", clips);
    AssimpLoader::loadWithArmature(importer3, "OBJFiles/Jump.fbx", clips);  
    AssimpLoader::loadWithArmature(importer4, "OBJFiles/Fall A Loop.fbx", clips);
    AssimpLoader::loadWithArmature(importer5, "OBJFiles/Standing Melee Attack Horizontal.fbx", clips);
    clips[ATTACK_ANIM].addEvent("DealDamage", 10);

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
    cube->addComponent<PlayerAttackController>();

    GameObject* stepTrigger = new GameObject(glm::vec3(3.f, -5.f, 0.f), glm::vec3(0.f), glm::vec3(2.1f,7.f,2.1f));
    stepTrigger->addComponent<MeshCollider>();
    stepTrigger->getComponent<MeshCollider>()->setAsTrigger();
    stepTrigger->getComponent<MeshCollider>()->initialize(cubeData[0].vertices, cubeData[0].indices);
    stepTrigger->addComponent<Follower>();
    stepTrigger->getComponent<Follower>()->setTarget(cube);
    cube->getComponent<CharacterController>()->addStepTrigger(stepTrigger->getComponent<MeshCollider>());
    collisionProcessor->addToPending(stepTrigger, stepTrigger->getComponent<MeshCollider>()->getRegion());

    this->gameObjects.push_back(stepTrigger);

    GameObject* box = new GameObject(glm::vec3(3.f, -6.f, 6.f), glm::vec3(0.f), glm::vec3(5.f,5.f,5.f));
    box->addComponent<Model>();
    box->addComponent<Follower>();
    box->getComponent<Follower>()->setTarget(cube);
    box->addComponent<MeshCollider>();
    box->getComponent<MeshCollider>()->setAsTrigger();
    box->getComponent<MeshCollider>()->initialize(cubeData[0].vertices, cubeData[0].indices);
    cubeData[0].br.collider = box->getComponent<MeshCollider>();
    collisionProcessor->addToPending(box, &cubeData[0].br);
    box->addComponent<DamageDealArea>();
    this->gameObjects.push_back(box);

    

    character->addComponent<CharacterAnimator>();
    character->getComponent<CharacterAnimator>()->setupStateMachine(clips, cube->getComponent<CharacterController>(), cube->getComponent<PlayerAttackController>());
    character->getComponent<CharacterAnimator>()->attachMesh(holoMesh);
    character->getComponent<CharacterAnimator>()->attachMesh(secondMesh);

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